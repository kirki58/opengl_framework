#pragma once

class Object{
    public:
        float* Vertices;
        Object(float* vertices){
            this->Vertices = vertices;
        }
};