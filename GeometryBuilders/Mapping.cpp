#include "Mapping.h"

Mapping::Mapping(shared_ptr<ConfigMappingReader> mr)
{
   setup = mr;
}


vec3 Mapping::mapeigPunt(vec3 puntMonReal) {
    // Mapeig del punt en coordenades reals a coordenades de mon virtual
      vec3 puntMonVirtual;

      puntMonVirtual.x = (setup->Vxmax - setup->Vxmin)*(puntMonReal.x - setup->Rxmin)/(setup->Rxmax - setup->Rxmin) +
                  setup->Vxmin;
      puntMonVirtual.y = (setup->Vymax - setup->Vymin)*(puntMonReal.y - setup->Rymin)/(setup->Rymax - setup->Rymin) +
              setup->Vymin;
      puntMonVirtual.z = (setup->Vzmax - setup->Vzmin)*(puntMonReal.z - setup->Rzmin)/(setup->Rzmax - setup->Rzmin) +
              setup->Vzmin;
      return puntMonVirtual;
}

float Mapping::mapeigValor(float valorMonReal) {
    // Mapeig del valor en dimensions reals a dimensions de mon virtual
    double valorMonVirtual;

    float rangRx = setup->Rxmax - setup->Rxmin;
    float rangRy = setup->Rymax - setup->Rymin;
    float rangRz = setup->Rzmax - setup->Rzmin;
    float rangR = min({rangRx, rangRy, rangRz});
    float rangVx = setup->Vxmax - setup->Vxmin;
    float rangVy = setup->Vymax - setup->Vymin;
    float rangVz = setup->Vzmax - setup->Vzmin;
    float rangV = min({rangVx, rangVy, rangVz});
    // Mapegem el valorMonReal
    valorMonVirtual = (valorMonReal / rangR)*rangV;

    return valorMonVirtual;

}


vec3 Mapping::mapeigPuntGizmo(vec3 puntMonReal) {
      vec3 puntMonVirtual;
      puntMonVirtual.x = (setup->Vxmax - setup->Vxmin)*(puntMonReal.x - setup->Rxmin)/(setup->Rxmax - setup->Rxmin) +
                      setup->Vxmin;
      puntMonVirtual.y = 0;
      puntMonVirtual.z = (setup->Vzmax - setup->Vzmin)*(puntMonReal.z - setup->Rzmin)/(setup->Rzmax - setup->Rzmin) +
                  setup->Vzmin;

      return puntMonVirtual;
}

float Mapping::mapeigEscalaGizmo(int i, float valorMonReal) {
    float valorMonVirtual;

    float rangVx = setup->Vxmax - setup->Vxmin;
    float rangVy = setup->Vymax - setup->Vymin;
    float rangVz = setup->Vzmax - setup->Vzmin;
    //Multipliquem per 0.5 perque pot ser un radi i volem que el gizmo estigui dins de l'escena virtual
    float rangV = 0.5f*min({rangVx, rangVy, rangVz});
    // Mapegem el valorMonReal en el rang [0, rangV]
    float propMin = setup->propLimits[i].first;
    float propMax = setup->propLimits[i].second;
    valorMonVirtual = ((valorMonReal - propMin)/(propMax - propMin)) * rangV;
    return valorMonVirtual;
}

shared_ptr<ColorMap> Mapping::getColorMap(ColorMapStatic::COLOR_MAP_TYPES tCM) {
   auto cm = make_shared<ColorMapStatic>(tCM);
   return cm;
}

shared_ptr<Material> Mapping::mapeigMaterial(int i, ColorMapStatic::COLOR_MAP_TYPES tCM, MaterialFactory::MATERIAL_TYPES mat_type, double valorMonReal) {

    auto cm = getColorMap(tCM);
    float idx = 255*((valorMonReal-setup->propLimits[i].first)/(setup->propLimits[i].second-setup->propLimits[i].first));
    if(tCM == ColorMapStatic::COLOR_MAP_TYPE_INFERNO) {
        return MaterialFactory::getInstance().createMaterial(vec3(0.1f), cm->getColor(idx), vec3(1.0f), 50, mat_type);
    } else if (tCM == ColorMapStatic::COLOR_MAP_TYPE_PLASMA) {
        return MaterialFactory::getInstance().createMaterial(vec3(0.1f), cm->getColor(idx), vec3(1.0f), 50, mat_type);
    } else if (tCM == ColorMapStatic::COLOR_MAP_TYPE_MAGMA) {
        return MaterialFactory::getInstance().createMaterial(vec3(0.1f), cm->getColor(idx), vec3(1.0f), 50, mat_type);
    } else if (tCM == ColorMapStatic::COLOR_MAP_TYPE_VIRIDIS) {
        return MaterialFactory::getInstance().createMaterial(vec3(0.1f), cm->getColor(idx), vec3(1.0f), 50, mat_type);
    } else if (tCM == ColorMapStatic::COLOR_MAP_TYPE_PARULA) {
        return MaterialFactory::getInstance().createMaterial(vec3(0.1f), cm->getColor(idx), vec3(1.0f), 50, mat_type);
    } else if (tCM == ColorMapStatic::COLOR_MAP_TYPE_JET) {
        return MaterialFactory::getInstance().createMaterial(vec3(0.1f), cm->getColor(idx), vec3(1.0f), 50, mat_type);
    }
}

