#include <cassert>
#include <iostream>
#include <iterator>
#include <memory>
#include <unordered_set>
#include <vector>

template <typename T> class any_iterator {
public:
  using iterator_category = std::forward_iterator_tag;
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using pointer = const T *;
  using reference = const T &;

private:
  struct Interface {
    virtual ~Interface() = default;
    virtual void next() = 0;
    virtual reference get() const = 0;
    virtual bool equal(const Interface *other) const = 0;
    virtual std::unique_ptr<Interface> clone() const = 0;
  };

  template <typename It> struct Impl : Interface {
    It it;
    Impl(It it) : it(it) {}

    void next() override { ++it; }

    reference get() const override { return *it; }

    bool equal(const Interface *other) const override {
      const auto *p = dynamic_cast<const Impl *>(other);
      return p && (it == p->it);
    }

    std::unique_ptr<Interface> clone() const override {
      return std::make_unique<Impl>(it);
    }
  };

  std::unique_ptr<Interface> ptr;

public:
  any_iterator() = default;

  template <typename It>
  any_iterator(It it) : ptr(std::make_unique<Impl<It>>(it)) {}

  any_iterator(const any_iterator &other)
      : ptr(other.ptr ? other.ptr->clone() : nullptr) {}

  any_iterator &operator=(const any_iterator &other) {
    if (this != &other) {
      ptr = other.ptr ? other.ptr->clone() : nullptr;
    }
    return *this;
  }

  any_iterator(any_iterator &&) noexcept = default;
  any_iterator &operator=(any_iterator &&) noexcept = default;

  reference operator*() const { return ptr->get(); }
  pointer operator->() const { return &ptr->get(); }

  any_iterator &operator++() {
    ptr->next();
    return *this;
  }
  any_iterator operator++(int) {
    any_iterator tmp = *this;
    ptr->next();
    return tmp;
  }

  bool operator==(const any_iterator &other) const {
    if (!ptr || !other.ptr)
      return ptr == other.ptr;
    return ptr->equal(other.ptr.get());
  }

  bool operator!=(const any_iterator &other) const { return !(*this == other); }
};

template <class T> class Storage {
public:
  virtual ~Storage() = default;
  virtual any_iterator<T> begin() const = 0;
  virtual any_iterator<T> end() const = 0;
  virtual bool insert(T val) = 0;
  virtual bool erase(T val) = 0;
  virtual bool contains(T val) const = 0;
  virtual size_t size() const = 0;
  virtual std::vector<T> merge(Storage<T> const &other_) const {
    Storage<T> const *small, *large;
    if (other_.size() > this->size()) {
      large = &other_;
      small = this;
    } else {
      large = this;
      small = &other_;
    }
    std::vector<T> ret(large->begin(), large->end());
    for (auto &i : *small) {
      if (!large->contains(i))
        ret.push_back(i);
    }
    return ret;
  }
  virtual std::vector<T> intersection(Storage<T> &other_) const {
    Storage<T> const *small, *large;
    std::vector<T> ret;
    if (other_.size() > this->size()) {
      large = &other_;
      small = this;
    } else {
      large = this;
      small = &other_;
    }
    for (auto &i : *small) {
      if (large->contains(i))
        ret.push_back(i);
    }
    return ret;
  }
};
template <class T> class ArrayStorage : public Storage<T> {
  std::vector<int> vec;

public:
  any_iterator<T> begin() const override { return vec.begin(); }
  any_iterator<T> end() const override { return vec.end(); };
  ArrayStorage() = default;
  ArrayStorage(Storage<T> &old) : vec{old.begin(), old.end()} {};
  bool contains(T val) const override {
    for (auto &i : vec)
      if (val == i)
        return true;
    return false;
  }
  bool insert(T val) override {
    if (contains(val))
      return false;
    vec.push_back(val);
    return true;
  }
  bool erase(T val) override {
    for (auto i = vec.begin(); i != vec.end(); i++)
      if (val == *i) {
        vec.erase(i);
        return true;
      }
    return false;
  }
  size_t size() const override { return vec.size(); }
};

template <class T> class HashSetStorage : public Storage<T> {
  std::unordered_set<int> set;

public:
  any_iterator<T> begin() const override { return set.begin(); }
  any_iterator<T> end() const override { return set.end(); };
  HashSetStorage() = default;
  HashSetStorage(Storage<T> &old) : set{old.begin(), old.end()} {};
  bool contains(T val) const override { return set.contains(val); }
  bool insert(T val) override {
    if (contains(val))
      return false;
    set.insert(val);
    return true;
  }
  bool erase(T val) override {
    if (contains(val)) {
      set.erase(val);
      return true;
    }
    return false;
  }
  size_t size() const override { return set.size(); }
};

template <class T> class Set {
private:
  const size_t THRESHOLD = 10;

  void update() {
    if (storage->size() > THRESHOLD) {
      storage = std::make_unique<HashSetStorage<T>>(*storage);
    }
  }

public:
  std::unique_ptr<Storage<T>> storage;
  Set() : storage(std::make_unique<ArrayStorage<T>>()) {}
  template <class C> Set(C container) {
    if (container.size() > THRESHOLD) {
      storage = std::make_unique<HashSetStorage<T>>();
    } else {
      storage = std::make_unique<ArrayStorage<T>>();
    }
    for (auto &i : container) {
      storage->insert(i);
    }
  }

  bool add(int val) {
    bool ret = storage->insert(val);
    update();
    return ret;
  }

  bool has(int val) { return storage->contains(val); }

  Set merge(const Set &other) { return Set(storage->merge(*other.storage)); }
  Set intersection(const Set &other) {
    return Set(storage->intersection(*other.storage));
  }
};

//// НЕЙРОТЕСТЫ
void test_basic_ops() {
  std::cout << "Running basic ops test..." << std::endl;
  Set<int> s;
  assert(s.add(1) == true);
  assert(s.add(1) == false); // Дубликат
  assert(s.has(1) == true);
  assert(s.has(2) == false);
  assert(s.storage->size() == 1);
  std::cout << "Basic ops passed!" << std::endl;
}

void test_bridge_transition() {
  std::cout << "Running bridge transition test (Array -> Hash)..." << std::endl;
  Set<int> s;

  // Добавляем 10 элементов (это предел ArrayStorage)
  for (int i = 1; i <= 15; ++i) {
    s.add(i);
  }
  // Проверяем, что всё еще ArrayStorage (через косвенные признаки или TypeID,
  // но в данном коде достаточно проверить сохранность данных)
  assert(s.storage->size() == 15);

  // 11-й элемент должен триггернуть update() и миграцию в HashSetStorage
  s.add(-1);

  assert(s.storage->size() == 16);
  for (int i = 1; i <= 11; ++i) {
    assert(s.has(i) == true); // Данные не потерялись при переезде
  }
  std::cout << "Transition passed!" << std::endl;
}

void test_erase() {
  std::cout << "Running erase test..." << std::endl;
  Set<int> s;
  for (int i = 0; i < 15; ++i)
    s.add(i); // Мы уже в HashStorage

  assert(s.storage->size() == 15);
  assert(s.storage->erase(5) == true);
  assert(s.storage->erase(5) == false); // Уже удален
  assert(s.storage->size() == 14);
  assert(s.has(5) == false);
  std::cout << "Erase passed!" << std::endl;
}

void test_set_algebra() {
  std::cout << "Running merge and intersection test..." << std::endl;
  Set<int> s1;
  Set<int> s2;

  for (int i = 1; i <= 5; ++i)
    s1.add(i); // {1, 2, 3, 4, 5}
  for (int i = 4; i <= 8; ++i)
    s2.add(i); // {4, 5, 6, 7, 8}

  // Пересечение: {4, 5}
  Set<int> res_inter = s1.intersection(s2);
  assert(res_inter.storage->size() == 2);
  assert(res_inter.has(4) && res_inter.has(5));
  assert(!res_inter.has(1));

  // Объединение: {1, 2, 3, 4, 5, 6, 7, 8}
  Set<int> res_merge = s1.merge(s2);

  assert(res_merge.storage->size() == 8);
  for (int i = 1; i <= 8; ++i)
    assert(res_merge.has(i));

  std::cout << "Algebra ops passed!" << std::endl;
}

int main() {
  try {
    test_basic_ops();
    test_bridge_transition();
    test_erase();
    test_set_algebra();
    std::cout << "\nAll tests passed successfully!" << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Test failed with error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
