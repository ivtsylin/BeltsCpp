#pragma once

namespace RAII {

template <class T>
class Booking {
    
public:
    Booking(T* res_, int count_) : resource(res_), Id(count_) {};
    
    Booking() = default;
    Booking(const Booking&) = delete;
    Booking(Booking&& other) : resource(other.resource), Id(other.Id) {
        other.resource = nullptr;
        other.Id = 0;
    }

    Booking& operator=(const Booking&) = delete;
    Booking& operator=(Booking&& other) {
        Reset();
        
        resource = other.resource;
        Id = other.Id;
        
        other.resource = nullptr;
        other.Id = 0;
        
        return *this;
    }
    
    void Reset() {
        if(resource) resource->CancelOrComplete(*this);
        
        resource = nullptr;
        Id = 0;
    }
    
    ~Booking() {
        Reset();
    }
    
private:
    T* resource;
    int Id;
};

}
