#include "collision.h"
#include "generator.h"

void checkCollision() {
    checkCollisionPlayer();
    checkCollisionEnemy();
    checkEnemiesInFrame();
    checkBulletsInFrame();
}

void checkCollisionPlayer() {
    if(player.alive) {
        for(byte i=0; i<numberOfBullets; i++) {
            // First check if player and enemy/bullet collide on the x-coordinate
            // and then on the y-coordinate.
            if((abs(player.x-bullets[i].x) < player.width-3
                && bullets[i].x >= player.x)
                &&(abs(player.y-bullets[i].y) < player.height-1
                && bullets[i].y >= player.y)
                && !bullets[i].playersBullet && player.invincible == 0) {
                
                if(bullets[i].damage >= player.lifepoints) {
                    player.alive = false;
                    player.lives--;
                    bullets[i].alive = false;
                    break;
                }
                player.lifepoints -= bullets[i].damage;
                bullets[i].alive = false;
            }
        }
    }
    // Only check this one if the player did not die due to a bullet.
    if(player.alive) {
        for(byte i=0; i<numberOfEnemies; i++) {
            if(((enemies[i].x-player.x < player.width && enemies[i].x > player.x)
                || (player.x-enemies[i].x < enemies[i].width && enemies[i].x < player.x))
                && ((enemies[i].y-player.y < player.height && enemies[i].y > player.y)
                || (player.y-enemies[i].y < enemies[i].height && enemies[i].y < player.y))
                && player.invincible == 0) {

                player.alive = false;
                if(!boss_coming)
                    enemies[i].alive = false;
                player.lives--;
                break;
            }
        }
    }
    // Check for supplies.
    if(player.alive) {
        for(byte i=0; i<noOfSupplies; i++) {
            if((abs(player.x-supplies[i].x) < player.width
                && supplies[i].x >= player.x)
                &&(abs(player.y-supplies[i].y) < player.height-1
                && supplies[i].y >= player.y)) {

                supplies[i].alive = false;
                extra_tick = 160;
                // Each number is a placeholder for different supplies, 
                // e.g. extra live weapon upgrade or invincibility 
                byte type = random(0,8);
                // Make getting an extra life more unlikely
                if(type == 0 and random(0, 100) < 80)
                    type = 8;
               // which_extra = type;
                switch(type) {
                    case 0:
                        player.lives++;
                        break;
                    case 1:
                        if(player.speed < 2) {
                                player.speed++;
                        } else {
                                player.score += 5;
                        }
                        break;
                    case 2:
                        if(player.fireSpeed > 10) {
                                player.fireSpeed -= 10;
                        } else {
                                player.score += 5;
                        }
                        break;
                    case 3:
                        if(player.maxBullets < 20) {
                                player.maxBullets++;
                        } else {
                                player.score += 5;
                        }
                        break;
                    case 4:
                        if(player.bulletSpeed < 4) {
                                player.bulletSpeed++;
                        } else {
                                player.score += 5;
                        }
                        break;
                    case 5:
                        player.invincible = 16;
                        break;
                    case 6:
                        if(player.firetype < 4) {
                                player.firetype++;
                        } else {
                                player.score += 5;
                        }
                        break;
                    case 7:
                        if(player.fireSpeed > 10) {
                                player.fireSpeed -= 10;
                        } else {
                                player.score += 5;
                        }
                        break;
                    case 8:
                        player.score += 10;
                        break;
                    default:
                        player.lives++;
                }
            }
        }
    }
}

// Checks only collisions between bullets and enemies.
// Collisions of player and enemies are done in checkCollisionPlayer.
void checkCollisionEnemy() {
    for(byte i=0; i<numberOfEnemies; i++) {
        for(byte j=0; j<numberOfBullets; j++) {
            if(bullets[j].playersBullet
                && (abs(enemies[i].x-bullets[j].x) < enemies[i].width
                && bullets[j].x >= enemies[i].x)
                &&(abs(enemies[i].y-bullets[j].y) < enemies[i].height
                && bullets[j].y >= enemies[i].y)) {

                bullets[j].alive = false;
                if(enemies[i].lifepoints < bullets[j].damage){ 
                    enemies[i].alive = false;
                    player.score++;
                    if(enemies[i].shipType == 32) boss_coming = false;
                    if(enemies[i].supply && noOfSupplies < MAXSUPPLY) {
                        createSupply(enemies[i].x, enemies[i].y);
                    }
                } else {
                    enemies[i].lifepoints -= bullets[j].damage;
                }                    
            }
        }
    }
}

void checkEnemiesInFrame() {
    for(byte i=0; i<numberOfEnemies; i++) {
        if(enemies[i].shipType != 32 && (enemies[i].x > 128 || enemies[i].y > 64
            || enemies[i].x == 0 || enemies[i].y == 0)) {

            enemies[i].alive = false;
        }
    }
}

void checkBulletsInFrame() {
    for(byte i=0; i<numberOfBullets; i++) {
        if(bullets[i].x > 128 || bullets[i].y > 64
            || bullets[i].x == 0 || bullets[i].y == 0) {

            bullets[i].alive = false;
        }
    }
}

void checkSuppliesInFrame() {
    for(byte i=0; i<noOfSupplies; i++) {
        if(supplies[i].x > 128 || supplies[i].y > 64
            || supplies[i].x == 0 || supplies[i].y == 0) {

            supplies[i].alive = false;
        }
    }
}

void checkAlive() {
    if(!player.alive) {
        // Add an explosion for the player and delete all the others
        explosions[0].tick = 0;
        explosions[0].x = player.x + (player.width >> 1);
        explosions[0].y = player.y + (player.height >> 1);
        for(byte j = 1; j <= MAXENEMIES; j++) {
            explosions[j].tick = 255;
        }
    } else {
        // int is needed or else i <- 0-1 = 255
        for(int i=numberOfBullets-1; i>=0; i--) {
            if(!bullets[i].alive) {
                // player.bullets should always be bigger than 0 if a player bullet is available...
                if(bullets[i].playersBullet && player.bullets > 0) {
                    player.bullets--;
                }
                if(i<numberOfBullets-1) {
                    bullets[i] = bullets[numberOfBullets-1];
                }
                numberOfBullets--;
            }
        }
            
        // int is needed or else i <- 0-1 = 255
        for(int i=noOfSupplies-1; i>=0; i--) {
            if(!supplies[i].alive) {
                if(i<noOfSupplies-1) {
                    supplies[i] = supplies[noOfSupplies-1];
                }
                noOfSupplies--;
            }
        }
            
        // For all enemies, if he is not alive, check if he is within the frame.
        for(int i=numberOfEnemies-1; i>=0; i--) {
            if(!enemies[i].alive) {
                byte j=0;
                if(enemies[i].x <= 128
                    && enemies[i].y <= 64 && enemies[i].x > 0
                    && enemies[i].y > 0) {

                    // Search the first free slot in explosions and add a new explosion.
                    for(byte j = 0; j <= MAXENEMIES; j++) {
                        if(explosions[j].tick >= 75) {
                            explosions[j].tick = 0;
                            explosions[j].x = enemies[i].x + (enemies[i].width >> 1);
                            explosions[j].y = enemies[i].y + (enemies[i].height >> 1);
                            j = 255;
                        }
                    }
                }
                if(i<numberOfEnemies-1) {
                    enemies[i] = enemies[numberOfEnemies-1];
                }
                numberOfEnemies--;
            }
        }
    }
}
