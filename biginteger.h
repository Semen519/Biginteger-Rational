#include <utility>
#include <vector>
#include <string>

using std::vector;

struct BigInteger {
 private:

  const static int base = 1000000000;
  const static int des = 10;
  bool isNeg = false;
  vector<int> digits;

  void zeroChecker() {
    if ((digits.empty()) || (digits.size() == 1 && digits[0] == 0)) {
      digits = {0};
      isNeg = false;
    }
  }

  bool modLess(const BigInteger& num2) const {
    if (digits.size() < num2.digits.size()) { return true; }
    if (digits.size() > num2.digits.size()) { return false; }
    for (size_t i = digits.size(); i > 0; --i) {
      if (digits[i - 1] < num2.digits[i - 1]) { return true; }
      if (digits[i - 1] > num2.digits[i - 1]) { return false; }
    }
    return false;
  }

  void swap(BigInteger& other) {
    std::swap(digits, other.digits);
    std::swap(isNeg, other.isNeg);
  }

  void minusMod(const BigInteger& other) {
    for (size_t i = 0; i < other.digits.size(); ++i) {
      if (digits[i] < other.digits[i]) {
        size_t j = i + 2;
        while(digits[j - 1] == 0) { ++j; }
        --digits[j-- - 1];
        while(j > i) { digits[j-- - 1] += base - 1; }
        digits[i]++;
      }
      digits[i] -= other.digits[i];
    }
    for (size_t i = digits.size(); i > 1; --i) {
      if (digits[i - 1] == 0) { digits.pop_back(); } else { break; }
    }
    zeroChecker();
  }

  void plusModDig(const BigInteger& other, size_t dig) {
    if (digits.size() < other.digits.size() + dig + 1) {
      digits.resize(other.digits.size() + dig + 1, 0);
    }
    for (size_t i = 0; i < other.digits.size(); ++i) {
      if (i + dig == digits.size()) { digits.push_back(0); }
      digits[i + dig] += other.digits[i];
      if (digits[i + dig] >= base) {
        digits[i + dig] %= base;
        if (i + dig + 1 == digits.size()) { digits.push_back(0); }
        ++digits[i + dig + 1];
      }
    }
    if (digits[digits.size() - 1] == 0) { digits.pop_back(); }
    zeroChecker();
  }

  void plusMod(const BigInteger& other) { plusModDig(other, 0); }

  BigInteger multiplyDigit(int digit) const {
    BigInteger copy = *this;
    if (digit == 0) {
      copy.digits = {0};
      copy.isNeg = false;
      return copy;
    }
    long long s = 0;
    for (size_t i = 0; i < digits.size(); ++i) {
      s += static_cast<long long>(digit) * static_cast<long long>(digits[i]);
      copy.digits[i] = static_cast<int>(s % base);
      s /= base;
    }
    if (s != 0) { copy.digits.push_back(static_cast<int>(s)); }
    return copy;
  }

 public:

  BigInteger() : isNeg(false), digits({0}) {}
  BigInteger(const int x1) : isNeg(false), digits({}) {
    int x = x1;
    if (x < 0) {
      isNeg = true;
      x = -x;
    }
    digits.push_back(x % base);
    if (x / base > 0) { digits.push_back(x / base); }
  }
  BigInteger(const std::string str) {
    isNeg = (str[0] == '-');
    size_t begin = (str[0] == '-' ? 1 : 0);
    while (begin < str.size() && str[begin] == '0') { ++begin; }
    size_t size = (str.size() + des - 2 - begin) / (des - 1);
    if (begin != str.size()) {
      digits.resize(size);
    } else {
      digits.resize(1);
      digits[0] = 0;
      isNeg = false;
    }
    int dig = 0;
    for (size_t i = str.size() + des - 1; i > begin + des - 1; i -= des - 1) {
      digits[dig] = 0;
      int tens = 1;
      for (size_t j = 0; j < des - 1 && i - j > begin + des - 1; ++j) {
        digits[dig] += (str[i - des - j] - '0') * tens;
        tens *= des;
      }
      ++dig;
    }
  }

  BigInteger& operator+=(const BigInteger& other) {
    if (!isNeg && !other.isNeg) {
      plusMod(other);
      return *this;
    }
    if (isNeg && other.isNeg) {
      plusMod(other);
      return *this;
    }
    if (isNeg && !other.isNeg) {
      if (modLess(other)) {
        isNeg = false;
        BigInteger copy = other;
        swap(copy);
        minusMod(copy);
      } else { minusMod(other); }
      return *this;
    }
    if (!isNeg && other.isNeg) {
      if (modLess(other)) {
        isNeg = true;
        BigInteger copy = other;
        swap(copy);
        minusMod(copy);
      } else { minusMod(other); }
    }
    return *this;
  }

  BigInteger& operator-=(const BigInteger& other) {
    BigInteger opposite = other;
    opposite.isNeg = !other.isNeg;
    *this += opposite;
    return *this;
  }

  BigInteger& operator*=(const BigInteger& num) {
    BigInteger copy;
    if ((num.digits[0] == 0 && num.digits.size() == 1) || (digits[0] == 0 && digits.size() == 1)) {
      copy.digits.clear();
      copy.digits.push_back(0);
      copy.isNeg = false;
      swap(copy);
      return *this;
    }
    copy.isNeg = (isNeg != num.isNeg);
    copy.digits.resize(digits.size() + num.digits.size(), 0);
    for (size_t i = 0; i < num.digits.size(); ++i) {
      copy.plusModDig(multiplyDigit(num.digits[i]), i);
    }
    if (digits[digits.size() - 1] == 0) { digits.pop_back(); }
    swap(copy);
    zeroChecker();
    return *this;
  }

  std::string toString() const {
    std::string answer;
    if (digits.empty()) {
      answer += '0';
      return answer;
    }
    if (isNeg) { answer.push_back('-'); }
    size_t i = digits.size();
    answer += std::to_string(digits[i - 1]);
    --i;
    while (i > 0) {
      short k = 0;
      int deg = 1;
      while (digits[i - 1] >= deg) {
        deg *= des;
        ++k;
      }
      std::string zeros(des - 1 - k, '0');
      answer += zeros;
      if (digits[i - 1] != 0) {answer += std::to_string(digits[i - 1]); }
      --i;
    }
    return answer;
  }

  BigInteger& operator++() {
    *this += 1;
    return *this;
  }

  BigInteger operator++(int) {
    BigInteger copy = *this;
    ++*this;
    return copy;
  }

  BigInteger& operator--() {
    BigInteger x(1);
    *this -= x;
    return *this;
  }

  BigInteger operator--(int) {
    BigInteger copy = *this;
    --*this;
    return copy;
  }

  explicit operator bool() const { return !(digits.size() == 1 && digits[0] == 0); }

  int binFind(const BigInteger& num2, int l, int r) {
    if (l == r) { return l; }
    int m = (l + r + 1) / 2;
    if (!modLess(num2.multiplyDigit(m))) { l = m; } else { r = m - 1; }
    return binFind(num2, l, r);
  }

  BigInteger& operator/=(const BigInteger& numx) {
    BigInteger num = numx;
    num.isNeg = false;
    if (modLess(num)) {
      digits.clear();
      isNeg = false;
      digits.push_back(0);
      return *this;
    }
    BigInteger ans;
    BigInteger num1;
    ans.isNeg = (isNeg != numx.isNeg);
    num1.digits.resize(num.digits.size());
    for (size_t i = 0; i < num.digits.size(); ++i) {
      num1.digits[i] = digits[digits.size() - num.digits.size() + i];
    }
    if (num1.modLess(num)) {
      num1.digits.insert(num1.digits.begin(), digits[digits.size() - num.digits.size() - 1]);
    }
    ans.digits.resize(digits.size() - num1.digits.size() + 1);
    for (size_t i = ans.digits.size(); i > 0; --i) {
      ans.digits[i - 1] = num1.binFind(num, 0, base - 1);
      BigInteger x = num.multiplyDigit(ans.digits[i - 1]);
      num1 -= x;
      if (num1.digits.size() == 1 && num1.digits[0] == 0) { num1.digits.clear(); }
      if (i > 1) { num1.digits.insert(num1.digits.begin(), digits[i - 2]); }
    }
    swap(ans);
    zeroChecker();
    return *this;
  }

  BigInteger& operator%=(const BigInteger& num) {
    BigInteger copy = *this;
    copy /= num;
    copy *= num;
    *this -= copy;
    return *this;
  }

  BigInteger operator-() const {
    if (digits.size() == 1 && digits[0] == 0) { return *this; }
    BigInteger opposite = *this;
    opposite.isNeg = !isNeg;
    return opposite;
  }

  bool isNegOut() const { return isNeg; }
  int Base() const { return base; }
  vector<int> digitsOut() const { return digits; }

};

bool modLess(const BigInteger& num1, const BigInteger& num2) {
  if (num1.digitsOut().size() < num2.digitsOut().size()) { return true; }
  if (num1.digitsOut().size() > num2.digitsOut().size()) { return false; }
  for (size_t i = num1.digitsOut().size(); i > 0; --i) {
    if (num1.digitsOut()[i - 1] < num2.digitsOut()[i - 1]) { return true; }
    if (num1.digitsOut()[i - 1] > num2.digitsOut()[i - 1]) { return false; }
  }
  return false;
}

bool operator<(const BigInteger& num1, const BigInteger& num2) {
  if (num1.isNegOut() && !num2.isNegOut()) { return true; }
  if (!num1.isNegOut() && num2.isNegOut()) { return false; }
  if (!num1.isNegOut() && !num2.isNegOut()) { return modLess(num1, num2); }
  return modLess(num2, num1);
}

bool operator==(const BigInteger& num1, const BigInteger& num2) {
  return (num1.digitsOut() == num2.digitsOut() && num1.isNegOut() == num2.isNegOut());
}

bool operator>(const BigInteger& num1, const BigInteger& num2) { return num2 < num1; }
bool operator<=(const BigInteger& num1, const BigInteger& num2) { return !(num2 < num1); }
bool operator>=(const BigInteger& num1, const BigInteger& num2) { return !(num1 < num2); }
bool operator!=(const BigInteger& num1, const BigInteger& num2) { return !(num1 == num2); }

BigInteger operator+(const BigInteger& num1, const BigInteger& num2) {
  BigInteger result = num1;
  result += num2;
  return result;
}

BigInteger operator-(const BigInteger& num1, const BigInteger& num2) {
  BigInteger copy = num1;
  copy -= num2;
  return copy;
}

BigInteger operator*(const BigInteger& num1, const BigInteger& num2) {
  BigInteger result = num1;
  result *= num2;
  return result;
}

BigInteger operator/(const BigInteger& num1, const BigInteger& num2) {
  BigInteger ans = num1;
  ans /= num2;
  return ans;
}

BigInteger operator%(const BigInteger& num1, const BigInteger& num2) {
  BigInteger ans = num1;
  ans %= num2;
  return ans;
}

std::ostream& operator<<(std::ostream& out, const BigInteger& num) {
  out << num.toString();
  return out;
}

std::istream& operator>>(std::istream& in, BigInteger& num) {
  std::string str;
  in >> str;
  num  = str;
  return in;
}

BigInteger operator""_bi(const char* cstr) {
  std::string str = cstr;
  return str;
}

BigInteger gcd(BigInteger& num1, BigInteger& num2) {
  if (num2 == 0) { return num1; }
  num1 %= num2;
  return gcd(num2, num1);
}

struct Rational {
 private:

  BigInteger numer = 1;
  BigInteger denom = 1;

  void reduction() {
    BigInteger copy_n = numer >= 0 ? numer : -numer;
    BigInteger copy_d = denom >= 0 ? denom : -denom;
    BigInteger gcdNum = (copy_n > copy_d ? gcd(copy_n, copy_d) : gcd(copy_d, copy_n));
    numer /= gcdNum;
    denom /= gcdNum;
  }

 public:

  Rational() : numer(1), denom(1) {}
  Rational(const BigInteger num) : numer(num), denom(1) {}
  Rational(const int num) : numer(num), denom(1) {}

  Rational& operator*=(const Rational& frac) {
    numer *= frac.numer;
    denom *= frac.denom;
    reduction();
    return *this;
  }

  Rational& operator-=(const Rational& frac) {
    numer = numer * frac.denom - denom * frac.numer;
    denom *= frac.denom;
    reduction();
    return *this;
  }

  Rational& operator+=(const Rational& frac) {
    numer = numer * frac.denom + denom * frac.numer;
    denom *= frac.denom;
    reduction();
    return *this;
  }

  Rational& operator/=(const Rational& frac) {
    numer *= frac.denom;
    denom *= frac.numer;
    if (denom < 0) {
      numer = -numer;
      denom = -denom;
    }
    reduction();
    return *this;
  }

  std::string toString() {
    reduction();
    std::string str1 = numer.toString();
    std::string str2 = denom.toString();
    return denom == 1 ? str1 : str1 + "/" + str2;
  }

  std::string asDecimal(size_t precision) const {
    std::string str = "";
    BigInteger deg = 1;
    static const int des = 10;
    for (size_t i = 0; i < precision; ++i) { deg *= 10; }
    BigInteger before = numer;
    BigInteger after = denom;
    before *= deg;
    before /= denom;
    after = before % deg;
    before /= deg;
    if (after < 0) { after = -after; }
    if (before == 0 && numer < 0) { str += '-'; }
    str += before.toString() + '.';
    if (after != 0) {
      for (BigInteger i = after * des; i < deg; i *= des) { str += '0'; }
      str += after.toString();
    } else { for (size_t i = 0; i < precision; ++i) { str += '0'; } }
    return str;
  }

  explicit operator double() const {
    std::string str = asDecimal(20);
    return stod(str);
  }

  Rational operator-() const {
    Rational copy = *this;
    copy.numer = -copy.numer;
    return copy;
  }

  BigInteger denomOut() const { return denom; }
  BigInteger numerOut() const { return numer; }

};

Rational operator*(const Rational& frac1, const Rational& frac2) {
  Rational copy = frac1;
  copy *= frac2;
  return copy;
}

Rational operator/(const Rational& frac1, const Rational& frac2) {
  Rational copy = frac1;
  copy /= frac2;
  return copy;
}

Rational operator-(const Rational& frac1, const Rational& frac2) {
  Rational copy = frac1;
  copy -= frac2;
  return copy;
}

Rational operator+(const Rational& frac1, const Rational& frac2) {
  Rational copy = frac1;
  copy += frac2;
  return copy;
}

bool operator<(const Rational& frac1, const Rational& frac2) {
  return frac1.numerOut() * frac2.denomOut() < frac2.numerOut() * frac1.denomOut();
}

bool operator==(const Rational& frac1, const Rational& frac2) {
  return frac1.numerOut() * frac2.denomOut() == frac2.numerOut() * frac1.denomOut();
}

bool operator>(const Rational& frac1, const Rational& frac2) { return frac2 < frac1; }
bool operator!=(const Rational& frac1, const Rational& frac2) { return !(frac1 == frac2); }
bool operator<=(const Rational& frac1, const Rational& frac2) { return !(frac2 < frac1); }
bool operator>=(const Rational& frac1, const Rational& frac2) { return !(frac1 < frac2); }