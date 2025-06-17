#pragma once

namespace YoulaiCache {

    template <typename Key, typename Value>
    class CachePolicy {
    public:
        virtual ~CachePolicy() {};

        // ��ӻ���ӿ�
        virtual void put(const Key& key, const Value& value) = 0;

        // key�Ǵ������  ���ʵ���ֵ�Դ�����������ʽ���� | ���ʳɹ�����true
        virtual bool get(const Key& key, Value& value) = 0;

        // ������������ҵ�key����ֱ�ӷ���value
        virtual Value get(const Key& key) = 0;

    };

} // namespace YoulaiCache