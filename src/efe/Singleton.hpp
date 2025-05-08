#pragma once

namespace efe
{
    template <typename T>
    class Singleton
    {
    public:
        /**
         * @brief Retorna uma referência para a instância única da classe.
         * 
         * @return T&
         */
        static T& getInstance()
        {
            static T instance;
            return instance;
        }

        Singleton(const Singleton&) = delete;
        Singleton& operator=(const Singleton&) = delete;

    protected:
        Singleton() = default;
        ~Singleton() = default;
    };
}
