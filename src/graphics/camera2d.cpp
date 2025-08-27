#include "camera2d.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Fusion
{
    Camera2D::Camera2D()
        : target({0.0f, 0.0f}), offset({0.0f, 0.0f}), rotation(0.0f), zoom(1.0f)
    {
    }

    glm::mat4 Camera2D::GetViewMatrix() const
    {
        // A matriz de vista faz o oposto do que se espera: para mover a "câmera" para a direita,
        // o "mundo" inteiro precisa ser movido para a esquerda. Por isso as transformações são inversas.

        glm::mat4 view = glm::mat4(1.0f);

        // 4. Aplica o offset da tela (geralmente para centralizar o alvo)
        view = glm::translate(view, glm::vec3(offset.x, offset.y, 0.0f));
        // 3. Aplica o zoom
        view = glm::scale(view, glm::vec3(zoom, zoom, 1.0f));
        // 2. Aplica a rotação
        view = glm::rotate(view, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        // 1. Move o mundo para que o 'target' da câmera fique no centro da visão
        view = glm::translate(view, glm::vec3(-target.x, -target.y, 0.0f));

        return view;
    }
} // namespace Fusion