#include "Space.h"
#include "Projectile.h"

Space::Space(float gravity) {
    this->gravity = gravity;
	dynamicActors.clear();
	staticActors.clear();
}

void Space::addDynamicActor(Actor* actor) {
	dynamicActors.push_back(actor);
}

void Space::addStaticActor(Actor* actor) {
    staticActors.push_back(actor);
}

void Space::removeDynamicActor(Actor* actor) {
	dynamicActors.remove(actor);
}

void Space::removeStaticActor(Actor* actor) {
	staticActors.remove(actor);
}

void Space::update() {
    updatePlayerBorders();
	for (auto const& actor : dynamicActors) {
        // Aun no se han detectado choques
        actor->collisionDown = false;
        actor->outLeft = true;
        actor->outRight = true;

        // MoverDerecha / izquierda
		updateMoveRight(actor);
		updateMoveLeft(actor);
        // Mover arriba/abajo
        updateMoveTop(actor);
        updateMoveDown(actor);
	}
}

void Space::updateMoveRight(Actor* dynamicAct) {
    if (dynamicAct->vx > 0) {
        float possibleMovement = dynamicAct->vx;
        // El mejor "idealmente" vx partimos de ese

        for (auto const& staticAct : staticActors) {
            float rightDynamic = dynamicAct->x + dynamicAct->width / 2;
            float topDynamic = dynamicAct->y - dynamicAct->height / 2;
            float downDynamic = dynamicAct->y + dynamicAct->height / 2;

            float leftStatic = staticAct->x - staticAct->width / 2;
            float topStatic = staticAct->y - staticAct->height / 2;
            float downStatic = staticAct->y + staticAct->height / 2;

            // Alerta!, Elemento estático en la trayectoria.
            if ((rightDynamic + dynamicAct->vx) >= leftStatic
                && rightDynamic <= leftStatic
                && topStatic < downDynamic
                && downStatic > topDynamic) {

                // Comprobamos si la distancia al estático es menor
                // que nuestro movimientoPosible actual
                if (possibleMovement >= leftStatic - rightDynamic) {
                    // La distancia es MENOR que nuestro movimiento posible
                    // Tenemos que actualizar el movimiento posible a uno menor
                    possibleMovement = leftStatic - rightDynamic;
                }
            }
        }
        // Ya se han comprobado todos los estáticos
        dynamicAct->x = dynamicAct->x + possibleMovement;
    }
}


void Space::updateMoveLeft(Actor* dynamicAct) {
    if (dynamicAct->vx < 0) {
        float possibleMovement = dynamicAct->vx;
        // El mejor "idealmente" vx partimos de ese
        
        for (auto const& staticAct : staticActors) {
            float leftDynamic = dynamicAct->x - dynamicAct->width / 2;
            float topDynamic = dynamicAct->y - dynamicAct->height / 2;
            float downDynamic = dynamicAct->y + dynamicAct->height / 2;

            float rightStatic = staticAct->x + staticAct->width / 2;
            float topStatic = staticAct->y - staticAct->height / 2;
            float downStatic = staticAct->y + staticAct->height / 2;

            // Alerta!, Elemento estático en la trayectoria.
            if ((leftDynamic + dynamicAct->vx) <= rightStatic
                && leftDynamic >= rightStatic
                && topStatic < downDynamic
                && downStatic > topDynamic) {

                // Comprobamos si la distancia al estático es menor
                // que nuestro movimientoPosible actual
                if (possibleMovement <= rightStatic - leftDynamic) {
                    // La distancia es MENOR que nuestro movimiento posible
                    // Tenemos que actualizar el movimiento posible a uno menor
                    possibleMovement = rightStatic - leftDynamic;
                }

            }

        }
        // Ya se han comprobado todos los estáticos
        dynamicAct->x = dynamicAct->x + possibleMovement;
    }
}

void Space::updateMoveTop(Actor* dynamicAct) {
    if (dynamicAct->vy < 0) {
        float possibleMovement = dynamicAct->vy;
        // El mejor "idealmente" vy partimos de ese

        for (auto const& staticAct : staticActors) {
            float topDynamic = dynamicAct->y - dynamicAct->height / 2;
            float downDynamic = dynamicAct->y + dynamicAct->height / 2;
            float rightDynamic = dynamicAct->x + dynamicAct->width / 2;
            float leftDynamic = dynamicAct->x - dynamicAct->width / 2;

            float topStatic = staticAct->y - staticAct->height / 2;
            float downStatic = staticAct->y + staticAct->height / 2;
            float rightStatic = staticAct->x + staticAct->width / 2;
            float leftStatic = staticAct->x - staticAct->width / 2;

            // Alerta!, Elemento estático en la trayectoria.
            if ((topDynamic + dynamicAct->vy) <= downStatic
                && downDynamic > topStatic
                && leftDynamic < rightStatic
                && rightDynamic > leftStatic) {

                // Comprobamos si la distancia al estático es menor
                // que nuestro movimientoPosible actual
                if (possibleMovement <= downStatic - topDynamic) {
                    // La distancia es MENOR que nuestro movimiento posible
                    // Tenemos que actualizar el movimiento posible a uno menor
                    possibleMovement = downStatic - topDynamic;
                }
            }
        }
        // Ya se han comprobado todos los estáticos
        dynamicAct->y = dynamicAct->y + possibleMovement;
    }
}

void Space::updateMoveDown(Actor* dynamicAct) {
    if (dynamicAct->vy > 0) {
        float possibleMovement = dynamicAct->vy;
        // El mejor "idealmente" vy partimos de ese

        for (auto const& staticAct : staticActors) {
            float topDynamic = dynamicAct->y - dynamicAct->height / 2;
            float downDynamic = dynamicAct->y + dynamicAct->height / 2;
            float rightDynamic = dynamicAct->x + dynamicAct->width / 2;
            float leftDynamic = dynamicAct->x - dynamicAct->width / 2;

            float topStatic = staticAct->y - staticAct->height / 2;
            float downStatic = staticAct->y + staticAct->height / 2;
            float rightStatic = staticAct->x + staticAct->width / 2;
            float leftStatic = staticAct->x - staticAct->width / 2;

            // Alerta!, Elemento estático en la trayectoria.
            if ((downDynamic + dynamicAct->vy) >= topStatic
                && topDynamic < downStatic
                && leftDynamic < rightStatic
                && rightDynamic > leftStatic) {

                // Comprobamos si la distancia al estático es menor
                // que nuestro movimientoPosible actual
                if (possibleMovement >= topStatic - downDynamic) {
                    // La distancia es MENOR que nuestro movimiento posible
                    // Tenemos que actualizar el movimiento posible a uno menor
                    possibleMovement = topStatic - downDynamic;
                    dynamicAct->collisionDown = true;

                    if (rightDynamic <= rightStatic) {
                        dynamicAct->outRight = false;
                    }
                    if (leftDynamic >= leftStatic) {
                        dynamicAct->outLeft = false;
                    }
                }
            }
        }
        // Ya se han comprobado todos los estáticos
        dynamicAct->y = dynamicAct->y + possibleMovement;
    }
}

/* Hara que le jugador no se pueda mover mas alla de los limites de la pantalla */
void Space::updatePlayerBorders() {
    if (((mainPlayer->x >= (0 + mainPlayer->width / 2)) && mainPlayer->vx < 0) 
            || ((mainPlayer->x <= (WIDTH - mainPlayer->width / 2)) && mainPlayer->vx > 0)) {
        mainPlayer->x = mainPlayer->x + mainPlayer->vx;
    }
    if (((mainPlayer->y >= (0 + mainPlayer->height / 2)) && mainPlayer->vy < 0)
            || ((mainPlayer->y <= (HEIGHT - mainPlayer->height / 2)) && mainPlayer->vy > 0)) {
        mainPlayer->y = mainPlayer->y + mainPlayer->vy;
    }
}
