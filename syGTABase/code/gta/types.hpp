#pragma once
#include <cstdint>

#pragma pack(push, 1)
class src_vector
{
public:
    src_vector() = default;
    src_vector(float x, float y, float z) : x(x), y(y), z(z) {}

    src_vector operator+(const src_vector& other)
    {
        return src_vector(x + other.x, y + other.y, z + other.z);
    }

    src_vector operator-(const src_vector& other)
    {
        return src_vector(x - other.x, y - other.y, z - other.z);
    }

    inline src_vector elementwise_multiply(const src_vector& other)
    {
        return src_vector(x * other.x, y * other.y, z * other.z);
    }

    src_vector operator*(const float& other)
    {
        return src_vector(x * other, y * other, z * other);
    }

    float x{};
private:
    char m_padding1[0x04];
public:
    float y{};
private:
    char m_padding2[0x04];
public:
    float z{};
private:
    char m_padding3[0x04];
};
#pragma pack(pop)


union vector4
{
    struct
    {
        float x, y, z, w;
    };
    float data[4];

    vector4() = default;
    vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
};

union vector2
{
    struct
    {
        float x, y;
    };
    float data[2];

    vector2() = default;
    vector2(float x, float y) : x(x), y(y) {}
};

using Void = void;
using Any = int;
using Hash = std::uint32_t;
using Entity = std::int32_t;
using Player = std::int32_t;
using FireId = std::int32_t;
using Interior = std::int32_t;
using Ped = Entity;
using Vehicle = Entity;
using Cam = std::int32_t;
using Object = Entity;
using Pickup = Object;
using Blip = std::int32_t;
using Camera = Entity;
using ScrHandle = Entity;

using Vector4 = vector4;
using Vector3 = src_vector;
using Vector2 = vector2;