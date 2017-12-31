#pragma once

#include "Drawable.H"
#include "Rectangle.H"

#include <chrono>
#include <numeric>

using Acceleration = std::array<double, 3>;
using Velocity     = std::array<double, 3>;
using SpaceVector  = std::array<double, 3>;

// Time in mics since epoch
using Time = std::uint64_t;

namespace std {

template <typename T, size_t N>
array<T, N> operator+(const array<T, N>& lhs,
                      const array<T, N>& rhs) {
    array<T, N> ret;
    for (size_t i = 0; i < N; ++i) {
        ret[i] = lhs[i] + rhs[i];
    }
    return ret;
}

template <typename T, size_t N, typename K>
array<T, N> operator*(const array<T, N>& lhs,
                      const K& rhs) {
    array<T, N> ret;
    for (size_t i = 0; i < N; ++i) {
        ret[i] = lhs[i] * rhs;
    }
    return ret;
}

template <typename T, size_t N>
ostream& operator<<(ostream& os, const array<T, N>& arr) {
    os << "{";
    size_t i = 0;
    do {
        os << arr[i++];
    } while ((i < N) && (os << ", "));
    os << "}";
}

} // namespace std

struct IPhysicalObject {
    virtual void updateTime(Time t) = 0;
};

struct Universe {
    static void addObject(IPhysicalObject* object) {
        objects_.emplace_back(object);
    }

    static void update() {
        Time now = std::chrono::system_clock::now().time_since_epoch()
                 / std::chrono::microseconds(1);
        for(auto object : objects_) {
            object->updateTime(now);
        }
    }
    static std::vector<IPhysicalObject*> objects_;
};
std::vector<IPhysicalObject*> Universe::objects_;

template <typename T,
          typename... Forces>
struct PhysicalObject
: public virtual IPhysicalObject {
    PhysicalObject() {
        time_ = std::chrono::system_clock::now().time_since_epoch()
                / std::chrono::microseconds(1);
        Universe::addObject(this);
    }
    void updateTime(Time t) {
        auto diff   = t - time_;
        auto factor = static_cast<double>(diff)/1'000'000;
        auto delta  = velocity_ * factor;
        static_cast<T*>(this)->move(delta);
        velocity_ = velocity_ + (acceleration_ * factor);

        std::array<SpaceVector, sizeof...(Forces)>
            forces = {Forces::getAcceleration()...};
        SpaceVector init = {0,0,0};
        auto net = std::accumulate(begin(forces), end(forces), init);
        acceleration_ = net;
    }

    static const std::string name() { return "General Object"; }


private:
    Time time_;
    Velocity     velocity_;
    Acceleration acceleration_;
};


struct Ground
: PhysicalObject<Ground>, Rectangle {
    Ground(std::shared_ptr<ShaderProgram> shaderProgram)
        : Rectangle(shaderProgram) {

        }

};
