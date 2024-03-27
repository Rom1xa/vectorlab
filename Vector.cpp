#include "VectorNewDouble.h"
#include <climits>
#include <cstddef>

Vector::Vector(const ValueType *rawArray, const size_t size, float coef) {
  _data = new ValueType[size];
  _capacity = size;
  _size = size;
  _multiplicativeCoef = coef;
  for (int i = 0; i < size; i++) {
    _data[i] = rawArray[i];
  }
}

Vector::Vector(const Vector &other) {
  // _data = new ValueType[other._capacity];
  // _size = other._size;
  // _capacity = other._capacity;
  // _multiplicativeCoef = other._multiplicativeCoef;
  // for (int i = 0; i < other._size; i++) {
  //   _data[i] = other._data[i];
  // }
  *this = other;
}

Vector &Vector::operator=(const Vector &other) {
  if (this == &other) {
    return *this;
  }
  _data = new ValueType[other._size];
  _size = other._size;
  _capacity = other._size;
  _multiplicativeCoef = other._multiplicativeCoef;
  for (int i = 0; i < other._size; i++) {
    _data[i] = other._data[i];
  }
  return *this;
}

Vector::Vector(Vector &&other) noexcept {
  // _data = other._data;
  // _size = other._size;
  // _capacity = other._capacity;
  // _multiplicativeCoef = other._multiplicativeCoef;

  // other._data = nullptr;
  // other._size = 0;
  // other._capacity = 0;
  // other._multiplicativeCoef = 2.0f;
  *this = std::move(other);
}

Vector &Vector::operator=(Vector &&other) noexcept {
  if (this != &other) {
    _data = other._data;
    _size = other._size;
    _capacity = other._capacity;
    _multiplicativeCoef = other._multiplicativeCoef;

    other._data = nullptr;
    other._size = 0;
    other._capacity = 0;
    other._multiplicativeCoef = 0;
  }
  return *this;
}

Vector::~Vector() { delete[] _data; }

void Vector::pushBack(const ValueType &value) {
  if (_size >= _capacity) {
    if (_capacity == 0) {
      _capacity = 1;
    }

    if (_data == nullptr) {
      _data = new ValueType[1];
    }
    reserve(_capacity * _multiplicativeCoef);
  }
  _data[_size++] = value;
}

void Vector::pushFront(const ValueType &value) {
  if (_capacity == 0) {
    _capacity = (int)_multiplicativeCoef;
    _data = new ValueType[(int)_multiplicativeCoef];
    _size = 1;
    _data[0] = value;
    return;
  }
  if (_size == _capacity) {
    reserve(_capacity * _multiplicativeCoef);
  }
  for (int i = _size; i > 0; i--) {
    _data[i] = _data[i - 1];
  }
  _data[0] = value;
  _size++;
}

void Vector::insert(const ValueType &value, size_t pos) {
  insert(&value, 1, pos);
}

void Vector::insert(const ValueType *values, size_t size, size_t pos) {
  if (_capacity == 0) {
    _capacity = 1;
    if (_data == nullptr) {
      _data = new ValueType[1];
    }
    _size = 1;
  }

  if (_size + size > _capacity) {
    reserve(_size + size);
  }

  for (size_t i = _size; i > pos; --i) {
    _data[i + size - 1] = _data[i - 1];
  }

  for (size_t i = 0; i < size; ++i) {
    _data[pos + i] = values[i];
  }
  _size += size;
}

void Vector::insert(const Vector &vector, size_t pos) {
  insert(vector._data, vector._size, pos);
}

void Vector::popBack() {
  if (_size > 0) {
    _size--;
  } else {
    throw std::out_of_range("Vector is empty");
  }
}

void Vector::popFront() {
  if (_size == 0 || _capacity == 0) {
    throw std::out_of_range("Vector is empty");
  } 

  for (int i = 1; i < _size; ++i) {
    _data[i - 1] = _data[i];
  }
  _size--;
}

void Vector::erase(size_t pos, size_t count) {
  count = std::min(count, _size - pos);
  for (size_t i = pos; i < _size - count; ++i) {
    _data[i] = _data[i + count];
  }
  _size -= count;
}

void Vector::eraseBetween(size_t beginPos, size_t endPos) {
  erase(beginPos, endPos - beginPos);
}

size_t Vector::size() const { return _size; }
size_t Vector::capacity() const { return _capacity; }
double Vector::loadFactor() const {
  return static_cast<double>(_size) / static_cast<double>(_capacity);
}

ValueType &Vector::operator[](size_t idx) { return _data[idx]; }

const ValueType &Vector::operator[](size_t idx) const { return _data[idx]; }

long long Vector::find(const ValueType &value) const {
  for (size_t i = 0; i < _size; ++i) {
    if (_data[i] == value) {
      return i;
    }
  }
  return -1;
}

void Vector::reserve(size_t capacity) {
  if (capacity > _capacity) {
    ValueType *newData = new ValueType[capacity];
    for (size_t i = 0; i < _size; ++i) {
      newData[i] = _data[i];
    }
    delete[] _data;
    _data = newData;
    _capacity = capacity;
  }
}

void Vector::shrinkToFit() {
  ValueType *tmp = new ValueType[_size];
  for (size_t i = 0; i < _size; ++i) {
    tmp[i] = _data[i];
  }
  delete[] _data;
  _data = tmp;
  _capacity = _size;
}

Vector::Iterator::Iterator(ValueType *ptr) : _ptr(ptr) {}

ValueType &Vector::Iterator::operator*() { return *_ptr; }

const ValueType &Vector::Iterator::operator*() const { return *_ptr; }

ValueType *Vector::Iterator::operator->() { return _ptr; }

const ValueType *Vector::Iterator::operator->() const { return _ptr; }

Vector::Iterator Vector::Iterator::operator++() {
  ++_ptr;
  return *this;
}

Vector::Iterator Vector::Iterator::operator++(int) {
  Iterator tmp = *this;
  ++_ptr;
  return tmp;
}

bool Vector::Iterator::operator==(const Iterator &other) const {
  return _ptr == other._ptr;
}

bool Vector::Iterator::operator!=(const Iterator &other) const {
  return _ptr != other._ptr;
}

Vector::Iterator Vector::begin() { return Iterator(_data); }

Vector::Iterator Vector::end() { return Iterator(_data + _size); }