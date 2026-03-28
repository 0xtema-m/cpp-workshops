#include <initializer_list>
#include <stdexcept>
#include <sstream>

namespace ohtuzh {

    template<typename T, size_t Size>
    class array {
    public:
        using value_type = T;

        array() = default;

        array(std::initializer_list<T> il) {
            // if (il.size() > Size) {
            //     throw std::runtime_error(
            //         (std::stringstream() << "Initializer list size (" << il.size() << ") is grather than array size (" << Size << ")").str()
            //     );
            // }
            size_t i = 0;
            for (const T& val: il) {
                data_[i] = val;
                ++i;
            }
        }

        size_t size() const {
            return Size;
        }

        T& operator[](const size_t index) {
            return data_[index];
        }

        const T& operator[](const size_t index) const {
            return data_[index];
        }

        T& at(const size_t index) {
            // if (index >= Size) {
            //     throw std::runtime_error(
            //         (std::stringstream() << "Index out of a range! (" << index << " >= " << Size << ")").str()
            //     );
            // }
            return data_[index];
        }

        const T& at(const size_t index) const {
            // if (index >= Size) {
            //     throw std::runtime_error(
            //         (std::stringstream() << "Index out of a range! (" << index << " >= " << Size << ")").str()
            //     );
            // }
            return data_[index];
        }

        T* begin() {
            return data_;
        }

        const T* begin() const {
            return data_;
        }

        T* end() {
            return data_ + Size;
        }

        const T* end() const {
            return data_ + Size;
        }

        ~array() = default;

    private:
        T data_[Size] {};
    };

} // namespace ohtuzh
