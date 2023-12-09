#ifndef BINSER_BINARYSERIALIZER_H
#define BINSER_BINARYSERIALIZER_H

#include "pch.h"

namespace binser {
    namespace polices {
        template<typename Derived>
        struct IStorage {
            template<typename T>
            void read(const T &elem) {
                (static_cast<Derived *>(this))->readImpl((char *) &elem, sizeof(T));
            }

            template<typename T>
            void write(T &out) {
                (static_cast<Derived *>(this))->writeImpl((char *) &out, sizeof(T));
            }

            std::size_t writeIdx{0};
            std::size_t readIdx{0};

            std::array<char, 1024> bytes{};
        };

        template<std::size_t SZ>
        struct StaticStoragePolicy : IStorage<StaticStoragePolicy<SZ>> {
            using storage_type = IStorage<StaticStoragePolicy<SZ>>;

            void readImpl(char *elem, std::size_t sz) {
                assert(storage_type::readIdx + sz <= SZ && "Buffer overflow !");

                if (storage_type::readIdx + sz > SZ) {
                    return;
                }

                std::memcpy(elem, storage_type::bytes.data() + storage_type::readIdx, sz);
                storage_type::readIdx += sz;
            }

            void writeImpl(const char *out, std::size_t sz) {
                std::memcpy(storage_type::bytes.data() + storage_type::writeIdx, out, sz);
                storage_type::writeIdx += sz;
            }
        };

        struct DynamicStoragePolicy {
        private:
            static char *malloc(std::size_t sz) {
                void *ret = std::malloc(sz);

                if (!ret) {
                    exit(EXIT_FAILURE);
                }

                return reinterpret_cast<char *>(ret);
            }

            static char *realloc(void *ptr, std::size_t newSz) {
                void *ret = std::realloc(ptr, newSz);

                if (!ret) {
                    exit(EXIT_FAILURE);
                }

                return reinterpret_cast<char *>(ret);
            }
        };
    }

    template<typename StoragePolicy>
    class Serializer : public polices::IStorage<StoragePolicy> {
    public:
        template<typename T>
        void write(const T &elem) {
            polices::IStorage<StoragePolicy>::write(elem);
        }

        template<typename T>
        void read(T &out) {
            polices::IStorage<StoragePolicy>::read(out);
        }

        void write(const char *cstr) {
            size_t len = strlen(cstr);

            write(len);
            for (size_t i = 0; i < len; i++) {
                write(cstr[i]);
            }
        }

        void read(char *outCstr) {
            assert(outCstr != nullptr && "c_str is null !");
            size_t len;
            read(len);

            for (size_t i = 0; i < len; i++) {
                read(outCstr[i]);
            }
        }

        void write(const std::string &str) {
            write(str.length());
            for (auto &&c: str) {
                write(c);
            }
        }

        void read(std::string &out) {
            size_t size;
            read(size);

            for (int i = 0; i < size; i++) {
                char c;
                read(c);
                out.push_back(c);
            }
        }

        template<typename E>
        void write(const std::vector<E> &vec) {
            write(vec.size());
            for (auto &&elem: vec) {
                write(elem);
            }
        }

        template<typename E>
        void read(std::vector<E> &out) {
            size_t size;
            read(size);

            for (int i = 0; i < size; i++) {
                E c;
                read(c);
                out.push_back(c);
            }
        }

        template<typename K, typename V>
        void write(const std::unordered_map<K, V> &map) {
            write(map.size());

            for (const auto &[key, val]: map) {
                write(key);
                write(val);
            }
        }

        template<typename K, typename V>
        void read(std::unordered_map<K, V> &map) {
            size_t size;
            read(size);

            for (int i = 0; i < size; i++) {
                K key;
                read(key);

                V val;
                read(val);

                map[key] = val;
            }
        }

        template<typename T, std::size_t N>
        void write(const T (&arr)[N]) {
            for (int i = 0; i < N; i++) {
                write(arr[i]);
            }
        }

        template<typename T, std::size_t N>
        void read(T (&arr)[N]) {
            for (int i = 0; i < N; i++) {
                read(arr[i]);
            }
        }

        template<typename K, typename V>
        void write(const std::map<K, V> map) {
            write(map.size());

            for (const auto &[key, val]: map) {
                write(key);
                write(val);
            }
        }

        template<typename K, typename V>
        void read(std::map<K, V> &map) {
            size_t size;
            read(size);

            for (int i = 0; i < size; i++) {
                K key;
                read(key);

                V val;
                read(val);

                map[key] = val;
            }
        }

        template<typename K>
        void write(const std::set<K> &set) {
            write(set.size());
            for (auto &&key: set) {
                write(key);
            }
        }

        template<typename K>
        void read(std::set<K> &set) {
            size_t size;
            read(size);

            for (int i = 0; i < size; i++) {
                K tmp;
                read(tmp);
                set.insert(tmp);
            }
        }

        template<typename T>
        void write(std::stack<T> &stack) {
            std::vector<T> vec;

            while (!stack.empty()) {
                T elem = stack.top();
                vec.push_back(elem);
                stack.pop();
            }

            typename std::vector<T>::reverse_iterator rit = vec.rbegin();
            size_t size = vec.size();

            write(size);
            while (rit != vec.rend()) {
                write(*rit);
                rit++;
            }

            for (auto &&elem: vec) {
                stack.push(elem);
            }
        }

        template<typename T>
        void read(std::stack<T> &stack) {
            size_t size;
            read(size);

            for (int i = 0; i < size; i++) {
                T tmp;
                read(tmp);
                stack.push(std::move(tmp));
            }
        }

        template<typename T>
        void write(std::queue<T> &queue) {
            std::vector<T> vec;

            while (!queue.empty()) {
                vec.push_back(queue.front());
                queue.pop();
            }

            write(vec.size());
            for (auto &&elem: vec) {
                write(elem);
            }

            for (auto &&elem: vec) {
                queue.push(elem);
            }
        }

        template<typename T>
        void read(std::queue<T> &queue) {
            size_t size;
            read(size);

            for (int i = 0; i < size; i++) {
                T tmp;
                read(tmp);
                queue.push(std::move(tmp));
            }
        }

        template<typename T>
        void write(std::priority_queue<T> &queue) {
            std::vector<T> vec;

            while (!queue.empty()) {
                vec.push_back(queue.top());
                queue.pop();
            }

            write(vec.size());
            for (auto &&elem: vec) {
                write(elem);
            }

            for (auto &&elem: vec) {
                queue.push(elem);
            }
        }

        template<typename T>
        void read(std::priority_queue<T> &queue) {
            size_t size;
            read(size);

            for (int i = 0; i < size; i++) {
                T tmp;
                read(tmp);
                queue.push(std::move(tmp));
            }
        }

        template<typename T>
        void write(const std::deque<T> &deque) {
            std::vector<T> vec{deque.cbegin(), deque.cend()};

            write(vec.size());
            for (auto &&elem: vec) {
                write(elem);
            }
        }

        template<typename T>
        void read(std::deque<T> &deque) {
            size_t size;
            read(size);

            for (int i = 0; i < size; i++) {
                T tmp;
                read(tmp);
                deque.push_front(std::move(tmp));
            }
        }
    };
}

#endif
