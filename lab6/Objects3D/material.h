#ifndef MATERIAL_H
#define MATERIAL_H


class Material
{
public:
    Material();
    Material(float Kdif, float Ks) : Kdif(Kdif), Ks(Ks) {}


    float Kdif;
    float Ks;
};

#endif // MATERIAL_H
