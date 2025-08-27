#ifndef CAMERA2D_H
#define CAMERA2D_H

#include "fusion_math.h"
#include "glm/glm.hpp"

namespace Fusion
{
    class Camera2D
    {
    public:

        Camera2D();

        // O ponto no mundo para onde a câmera aponta (o centro da visão)
        Vector2f target;

        // O deslocamento da câmera na tela. Usado para centralizar o 'target'.
        // Para uma câmera que centraliza o alvo, offset = {larguraTela/2, alturaTela/2}
        Vector2f offset;

        // Rotação da câmera em graus
        float rotation;

        // Nível de zoom. 1.0f = normal. > 1.0f = mais zoom. < 1.0f = menos zoom.
        float zoom;

        // Calcula e retorna a matriz de transformação da câmera (View Matrix)
        glm::mat4 GetViewMatrix() const;
    };
} // namespace Fusion

#endif