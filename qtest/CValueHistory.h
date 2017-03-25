#ifndef CVALUE_HISTORY_H
#define CVALUE_HISTORY_H

#include <deque>

template<typename T, int M>
class CValueHistoryT {
 private:
  T             value_;
  std::deque<T> values_;
  uint          max_;

 public:
  CValueHistoryT() :
   max_(M) {
  }

  CValueHistoryT(const T &value) :
   value_(value), max_(M) {
  }

  void setValue(const T &value) {
    if (values_.size() >= max_)
      values_.pop_front();

    values_.push_back(value_);

    value_ = value;
  }

  void getValue(T &value) const {
    value = value_;
  }

  T value() const {
    return value_;
  }

  bool hasLastValue() const {
    return hasNthValue(1);
  }

  bool getLastValue(T &value) const {
    return getNthValue(1, value);
  }

  T lastValue() const {
    return nthValue(1);
  }

  bool hasNthValue(uint n) const {
    return (values_.size() >= n);
  }

  bool getNthValue(uint n, T &value) const {
    if (! hasNthValue(n)) return false;

    value = values_[values_.size() - n];
  }

  T nthValue(uint n) const {
    assert(hasNthValue(n));

    return values_[values_.size() - n];
  }

  bool valueChanged() const {
    return (hasLastValue() && value() != lastValue());
  }

  bool revertValue() {
    return revertNthValue(1);
  }

  bool revertNthValue(uint n) {
    if (! hasNthValue(n)) return false;

    for ( ; n > 1; --n)
      values_.pop_back();

    value_ = values_.back();

    values_.pop_back();

    return true;
  }

  //------

  friend bool operator<(const CValueHistoryT &lhs,
                        const CValueHistoryT &rhs) {
    return (lhs.value_ < rhs.value_);
  }

  friend bool operator>(const CValueHistoryT &lhs,
                        const CValueHistoryT &rhs) {
    return ! (lhs < rhs || lhs == rhs);
  }

  friend bool operator<=(const CValueHistoryT &lhs,
                         const CValueHistoryT &rhs) {
    return (lhs < rhs || lhs == rhs);
  }

  friend bool operator>=(const CValueHistoryT &lhs,
                         const CValueHistoryT &rhs) {
    return ! (lhs < rhs);
  }

  friend bool operator==(const CValueHistoryT &lhs,
                         const CValueHistoryT &rhs) {
    return (lhs.value_ == rhs.value_);
  }

  friend bool operator==(const CValueHistoryT &lhs, const T &rhs) {
    return (lhs.value_ == rhs);
  }

  friend bool operator!=(const CValueHistoryT &lhs,
                         const CValueHistoryT &rhs) {
    return ! (lhs == rhs);
  }

  friend bool operator!=(const CValueHistoryT &lhs, const T &rhs) {
    return ! (lhs == rhs);
  }

  //------

  void print(std::ostream &os) {
    os << value_;
  }

  friend std::ostream &operator<<(std::ostream &os, CValueHistoryT &v) {
    v.print(os);

    return os;
  }
};

#endif
