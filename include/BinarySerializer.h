#ifndef BINSER_BINARYSERIALIZER_H
#define BINSER_BINARYSERIALIZER_H

#include "pch.h"

namespace binser {
    namespace polices {
        struct DynamicStorage {

        };

        template<int sz>
        struct StaticStorage {
            std::array<char *, sz> m_storage;
        };

    }

    class Serializer {
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

    public:
        Serializer() {
            m_bytes = malloc(m_blockData.logSz);
        }

        Serializer(const Serializer &other) {
            if (this != &other) {
                m_blockData.logSz = other.m_blockData.logSz;
                m_blockData.phySz = other.m_blockData.phySz;
                m_readIdx = 0;
                m_bytes = malloc(m_blockData.logSz);
                memcpy(m_bytes, other.m_bytes, m_blockData.logSz);
            }
        }

        Serializer(Serializer &&other) noexcept {
            if (this != &other) {
                stealResources(std::move(other));
            }
        }

        Serializer &operator=(Serializer other) {
            stealResources(std::move(other));
            return *this;
        }

        ~Serializer() {
            std::free(m_bytes);
        }

        void saveToFile(const std::string &fileName) {
            std::ofstream outStream(fileName, std::ios_base::binary);

            if (outStream.is_open()) {
                outStream.write((char *) &m_blockData.phySz, sizeof(size_t));
                outStream.write((char *) m_bytes, (long long) m_blockData.phySz);
            }
        }

        void loadFromFile(const std::string &fileName) {
            std::ifstream inStream(fileName, std::ios_base::binary);

            if (inStream.is_open()) {
                inStream.read((char *) &m_blockData.phySz, sizeof(size_t));
                m_blockData.logSz = m_blockData.phySz * 2;
                free(m_bytes);
                m_bytes = malloc(m_blockData.logSz);
                inStream.read((char *) m_bytes, (long long) m_blockData.phySz);
            }
        }

        template<typename T>
        void write(const T &elem) {
            static_assert(std::is_trivial_v < T > , "Type is not trivial type");
            if (m_blockData.phySz + sizeof(T) >= m_blockData.logSz) {
                m_blockData.logSz *= 2;
                m_bytes = realloc(m_bytes, m_blockData.logSz);
            }

            memcpy((char *) m_bytes + m_blockData.phySz, &elem, sizeof(T));
            m_blockData.phySz += sizeof(T);
        }

        template<typename T>
        void read(T &out) {
            static_assert(std::is_trivial_v < T > , "Type is not trivial type");
            memcpy(&out, (char *) m_bytes + m_readIdx, sizeof(T));
            m_readIdx = (m_readIdx + sizeof(T)) % m_blockData.phySz;
        }

        void write(const char* cstr) {
            size_t len = strlen(cstr);

            write(len);
            for(size_t i = 0; i < len; i++) {
                write(cstr[i]);
            }
        }

        void read(char* outCstr) {
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
        void write(const std::vector <E> &vec) {
            write(vec.size());
            for (auto &&elem: vec) {
                write(elem);
            }
        }

        template<typename E>
        void read(std::vector <E> &out) {
            size_t size;
            read(size);

            for (int i = 0; i < size; i++) {
                E c;
                read(c);
                out.push_back(c);
            }
        }

        template<typename K, typename V>
        void write(const std::unordered_map <K, V> &map) {
            write(map.size());

            for (const auto &[key, val]: map) {
                write(key);
                write(val);
            }
        }

        template<typename K, typename V>
        void read(std::unordered_map <K, V> &map) {
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
        void write(const std::map <K, V> map) {
            write(map.size());

            for (const auto &[key, val]: map) {
                write(key);
                write(val);
            }
        }

        template<typename K, typename V>
        void read(std::map <K, V> &map) {
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
        void write(const std::set <K> &set) {
            write(set.size());
            for (auto &&key: set) {
                write(key);
            }
        }

        template<typename K>
        void read(std::set <K> &set) {
            size_t size;
            read(size);

            for (int i = 0; i < size; i++) {
                K tmp;
                read(tmp);
                set.insert(tmp);
            }
        }

        template<typename T>
        void write(std::stack <T> &stack) {
            std::vector <T> vec;

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
        void read(std::stack <T> &stack) {
            size_t size;
            read(size);

            for (int i = 0; i < size; i++) {
                T tmp;
                read(tmp);
                stack.push(std::move(tmp));
            }
        }

        template<typename T>
        void write(std::queue <T> &queue) {
            std::vector <T> vec;

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
        void read(std::queue <T> &queue) {
            size_t size;
            read(size);

            for (int i = 0; i < size; i++) {
                T tmp;
                read(tmp);
                queue.push(std::move(tmp));
            }
        }

        template<typename T>
        void write(std::priority_queue <T> &queue) {
            std::vector <T> vec;

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
        void read(std::priority_queue <T> &queue) {
            size_t size;
            read(size);

            for (int i = 0; i < size; i++) {
                T tmp;
                read(tmp);
                queue.push(std::move(tmp));
            }
        }

        template<typename T>
        void write(const std::deque <T> &deque) {
            std::vector <T> vec{deque.cbegin(), deque.cend()};

            write(vec.size());
            for (auto &&elem: vec) {
                write(elem);
            }
        }

        template<typename T>
        void read(std::deque <T> &deque) {
            size_t size;
            read(size);

            for (int i = 0; i < size; i++) {
                T tmp;
                read(tmp);
                deque.push_front(std::move(tmp));
            }
        }

    private:
        void stealResources(Serializer &&other) {
            std::swap(m_blockData, other.m_blockData);
            std::swap(m_readIdx, other.m_readIdx);
            std::swap(m_bytes, other.m_bytes);
            other.m_bytes = nullptr;
        }

    private:
        struct BlockData {
            size_t phySz{0};
            size_t logSz{4};
        };

        char *m_bytes{nullptr};
        BlockData m_blockData;

        size_t m_readIdx{0};
    };
}

#endif