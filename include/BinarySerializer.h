#ifndef BINSER_BINARYSERIALIZER_H
#define BINSER_BINARYSERIALIZER_H

#include "pch.h"

namespace binser {
    namespace polices {
        struct StaticStoragePolicy;
        struct DynamicStoragePolicy;

        namespace traits {
            template<typename T>
            struct is_dynamic_v : std::false_type {
            };

            template<>
            struct is_dynamic_v<DynamicStoragePolicy> : std::true_type {
            };

            template<typename T>
            struct is_static_v : std::false_type {
            };

            template<>
            struct is_static_v<StaticStoragePolicy> : std::true_type {
            };
        }

        template<typename Derived>
        struct IStorage {
            IStorage() {
                if constexpr (traits::is_static_v<Derived>::value) {
                    bytes.staticStorage = {};
                    control.staticControlBlock = {0, 0};
                } else {
                    control.dynamicControlBlock = {0, 4, 0};
                    bytes.dynamicStorage = new char[control.dynamicControlBlock.logSz];
                }
            }

            ~IStorage() {
                if constexpr (traits::is_dynamic_v<Derived>::value) {
                    delete[] bytes.dynamicStorage;
                }
            }

            template<typename T>
            void read(const T &elem) {
                (static_cast<Derived *>(this))->readImpl((char *) &elem, sizeof(T));
            }

            template<typename T>
            void write(T &out) {
                (static_cast<Derived *>(this))->writeImpl((char *) &out, sizeof(T));
            }


            union ControlBlock {
                struct StaticControlBlock {
                    std::size_t writeIdx;
                    std::size_t readIdx;
                } staticControlBlock;

                struct DynamicControlBlock {
                    std::size_t phySz;
                    std::size_t logSz;
                    std::size_t readIdx;
                } dynamicControlBlock;
            };

            union Storage {
                std::array<char, 1024> staticStorage;
                char *dynamicStorage;
            };

            ControlBlock control;
            Storage bytes;
        };

        struct StaticStoragePolicy : IStorage<StaticStoragePolicy> {
            using storage_type = IStorage<StaticStoragePolicy>;

            void readImpl(char *elem, std::size_t sz) {
                assert(storage_type::control.staticControlBlock.readIdx + sz <=
                       storage_type::bytes.staticStorage.size() && "Buffer overflow !");

                if (storage_type::control.staticControlBlock.readIdx + sz >= storage_type::bytes.staticStorage.size()) {
                    return;
                }

                std::memcpy(elem,
                            storage_type::bytes.staticStorage.data() + storage_type::control.staticControlBlock.readIdx,
                            sz);
                storage_type::control.staticControlBlock.readIdx += sz;
            }

            void writeImpl(const char *out, std::size_t sz) {
                std::memcpy(
                        storage_type::bytes.staticStorage.data() + storage_type::control.staticControlBlock.writeIdx,
                        out, sz);
                storage_type::control.staticControlBlock.writeIdx += sz;
            }
        };

        struct DynamicStoragePolicy : IStorage<DynamicStoragePolicy> {
            using storage_type = IStorage<DynamicStoragePolicy>;

            void readImpl(char *elem, std::size_t sz) {
                std::memcpy(elem,
                            storage_type::bytes.dynamicStorage + storage_type::control.dynamicControlBlock.readIdx,
                            sz);
                storage_type::control.dynamicControlBlock.readIdx =
                        (storage_type::control.dynamicControlBlock.readIdx + sz) %
                        storage_type::control.dynamicControlBlock.logSz;
            }

            void writeImpl(const char *out, std::size_t sz) {
                if (storage_type::control.dynamicControlBlock.phySz + sz >=
                    storage_type::control.dynamicControlBlock.logSz
                        ) {
                    storage_type::control.dynamicControlBlock.logSz *= 2;
                    auto *oldBytes = storage_type::bytes.dynamicStorage;
                    storage_type::bytes.dynamicStorage = new char[storage_type::control.dynamicControlBlock.logSz];
                    std::memcpy(storage_type::bytes.dynamicStorage, oldBytes,
                                storage_type::control.dynamicControlBlock.phySz);
                    delete[] oldBytes;
                }

                std::memcpy(
                        storage_type::bytes.dynamicStorage + storage_type::control.dynamicControlBlock.phySz,
                        out, sz);
                storage_type::control.dynamicControlBlock.phySz += sz;
            }
        };
    }

    template<typename StoragePolicy>
    class Serializer : public polices::IStorage<StoragePolicy> {
        using map_type = std::unordered_map<std::type_index, std::function<void(void *)>>;

    public:
        template<typename T>
        void write(const T &elem) {
            polices::IStorage<StoragePolicy>::write(elem);
        }

        template<typename T>
        void read(T &&out) {
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

        template<typename T>
        void write(T* ptr, std::size_t sz) {
            for (std::size_t i = 0; i < sz; i++) {
                write(ptr[i]);
            }
        }

        template<typename T>
        void read(T* out, std::size_t sz) {
            for (std::size_t i = 0; i < sz; i++) {
                read(out[i]);
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

        void defineTemplateRead(const std::type_index &info, const std::function<void(void *)> &func) {
            if (m_serializationTemplate.find(info) == m_serializationTemplate.end()) {
                m_serializationTemplate[info] = func;
            }
        }

        void defineTemplateWrite(const std::type_index &info, const std::function<void(void *)> &func) {
            if (m_deserializationTemplate.find(info) == m_deserializationTemplate.end()) {
                m_deserializationTemplate[info] = func;
            }
        }

        template<typename T>
        void readRegObject(T &elem) {
            auto obj = m_serializationTemplate.find(typeid(T));
            if (obj != m_serializationTemplate.end()) {
                obj->second((void *) &elem);
            }
        }

        template<typename T>
        void writeRegObject(T &elem) {
            auto obj = m_deserializationTemplate.find(typeid(T));
            if (obj != m_deserializationTemplate.end()) {
                obj->second((void *) &elem);
            }
        }

    private:
        map_type m_serializationTemplate;
        map_type m_deserializationTemplate;
    };

    using StaticBinSer = binser::Serializer<binser::polices::StaticStoragePolicy>;
    using DynamicBinSer = binser::Serializer<binser::polices::DynamicStoragePolicy>;
}

#endif
