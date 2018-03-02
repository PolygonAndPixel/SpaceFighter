#include "draw.h"

bool drawGame() {
    arduboy.clear();
    drawScore();
    drawLives();
    drawEnemies();
    if(player.alive) drawPlayer();
    drawSupply();
    drawBullets();
    drawStars();
    bool finished = drawExplosions();
    drawExtra();
    arduboy.display();
    return finished;
}

void showTitle() {
    bool showingTitle = true;
    while(showingTitle) {
        arduboy.clear();
        // X, Y, name, width, height, color
        arduboy.drawBitmap(0, 0, title, 128, 64, 1);
        arduboy.display();
        if(arduboy.pressed(A_BUTTON)) showingTitle = false;
        delay(200);
    }
}

void drawStars() {
    for(byte i=0; i<numberOfStars; i++) {
        arduboy.drawPixel(stars[i].x, stars[i].y, 1);
    }
}

void drawPlayer() {
    arduboy.drawBitmap(player.x, player.y, ship0, player.height, player.width, 1);
}

void drawEnemies() {
    for(byte i=1; i<=numberOfEnemies; i++) {
        switch(enemies[i-1].shipType) {
            case 1:
                arduboy.drawBitmap(enemies[i-1].x, enemies[i-1].y,
                    enemy0, enemies[i-1].width, enemies[i-1].height, 1);
                break;
            case 2:
                arduboy.drawBitmap(enemies[i-1].x, enemies[i-1].y,
                    enemy1, enemies[i-1].width, enemies[i-1].height, 1);
                break;
            case 4:
                arduboy.drawBitmap(enemies[i-1].x, enemies[i-1].y,
                    enemy2, enemies[i-1].width, enemies[i-1].height, 1);
                break;
            case 8:
                for(byte j=0; j<4; j++) 
                    arduboy.drawPixel(enemies[i-1].x+j, enemies[i-1].y+2, 1);
            
                arduboy.drawPixel(enemies[i-1].x+3, enemies[i-1].y+3, 1);
                arduboy.drawPixel(enemies[i-1].x+3, enemies[i-1].y+1, 1);
            
                arduboy.drawPixel(enemies[i-1].x+2, enemies[i-1].y+4, 1);
                arduboy.drawPixel(enemies[i-1].x+2, enemies[i-1].y, 1);     
                break;
            case 16:
                for(byte j=0; j<5; j++) {
                    arduboy.drawPixel(enemies[i-1].x+j, enemies[i-1].y+3, 1);
                    arduboy.drawPixel(enemies[i-1].x+j, enemies[i-1].y+4, 1);
                }
            
                for(byte j=0; j<3; j++) {
                    arduboy.drawPixel(enemies[i-1].x+2+j, enemies[i-1].y+j, 1);
                    arduboy.drawPixel(enemies[i-1].x+4-j, 
                        enemies[i-1].y+j+5, 1);
                }
                break;
            case 32:
                // The boss is drawn slightly differently
                arduboy.drawBitmap(enemies[i-1].x-19, enemies[i-1].y,
                    boss1, enemies[i-1].width+19, enemies[i-1].height, 1);
                break;
            default:
                arduboy.drawBitmap(enemies[i-1].x, enemies[i-1].y,
                    enemy2, enemies[i-1].width, enemies[i-1].height, 1);
        }
    }
}

void drawBullets() {
    for(byte i=0; i<numberOfBullets; i++) {
        arduboy.drawPixel(bullets[i].x, bullets[i].y, 1);
        arduboy.drawPixel(bullets[i].x-1, bullets[i].y, 1);
        arduboy.drawPixel(bullets[i].x, bullets[i].y-1, 1);
        arduboy.drawPixel(bullets[i].x-1, bullets[i].y-1, 1);
    }
}

void drawSupply() {
    for(byte i=0; i<noOfSupplies; i++) {
        if(supplies[i].alive) {
            arduboy.drawPixel(supplies[i].x, supplies[i].y, 1);
            arduboy.drawPixel(supplies[i].x-1, supplies[i].y, 1);
            arduboy.drawPixel(supplies[i].x+1, supplies[i].y, 1);
            arduboy.drawPixel(supplies[i].x, supplies[i].y+1, 1);
            arduboy.drawPixel(supplies[i].x, supplies[i].y-1, 1);

        }
    }
}

void drawExtra() {
    // It is a bit ugly, so I deactivated it for the moment
    extra_tick = 0;
    if(extra_tick > 0) {
        arduboy.setCursor(30, 15);
        switch(which_extra) {
            case 0:
                arduboy.print("Extra life");
                break;
            case 1:
                if(player.speed > 1) 
                    arduboy.print("More speed");
                break;
            case 2:
                if(player.fireSpeed > 10) 
                    arduboy.print("Shoot faster");
                break;
            case 3:
                if(player.maxBullets < 5) 
                    arduboy.print("Spam bullets");
                break;
            case 4:
                if(player.bulletSpeed < 4) 
                    arduboy.print("Faster bullets");
                break;
            case 5:
                arduboy.print("Invincible!!!");
                break;
            case 6:
                break;
            case 7:
                if(player.fireSpeed > 10) 
                    arduboy.print("Shoot faster");
                break;
            case 8:
                if(player.firetype < 5) 
                    arduboy.print("Shoot nicer");
                break;
            default:
                arduboy.print("Extra life");
        }
        extra_tick--;
    }
}

bool drawExplosions() {
    bool finished = true;
    // Not very efficiently...
    for(byte i=0; i<=MAXENEMIES; i++) {
        if(explosions[i].tick < 15) {
            finished = false;
            arduboy.drawPixel(explosions[i].x, explosions[i].y, 1);
            arduboy.drawPixel(explosions[i].x, explosions[i].y+1, 1);
            arduboy.drawPixel(explosions[i].x, explosions[i].y-1, 1);
            arduboy.drawPixel(explosions[i].x-1, explosions[i].y, 1);
            arduboy.drawPixel(explosions[i].x+1, explosions[i].y, 1);
            explosions[i].tick++;
        } else if(explosions[i].tick < 30) {
            finished = false;
            arduboy.drawPixel(explosions[i].x, explosions[i].y, 1);
            arduboy.drawPixel(explosions[i].x, explosions[i].y+1, 1);
            arduboy.drawPixel(explosions[i].x, explosions[i].y-1, 1);
            arduboy.drawPixel(explosions[i].x-1, explosions[i].y, 1);
            arduboy.drawPixel(explosions[i].x+1, explosions[i].y, 1);

            arduboy.drawPixel(explosions[i].x, explosions[i].y+2, 1);
            arduboy.drawPixel(explosions[i].x, explosions[i].y-2, 1);
            arduboy.drawPixel(explosions[i].x-2, explosions[i].y, 1);
            arduboy.drawPixel(explosions[i].x+2, explosions[i].y, 1);
            // diagonal lines
            arduboy.drawPixel(explosions[i].x+1, explosions[i].y+1, 1);
            arduboy.drawPixel(explosions[i].x+1, explosions[i].y-1, 1);
            arduboy.drawPixel(explosions[i].x-1, explosions[i].y+1, 1);
            arduboy.drawPixel(explosions[i].x-1, explosions[i].y-1, 1);

            explosions[i].tick++;
        } else if(explosions[i].tick < 45) {
            finished = false;
            arduboy.drawPixel(explosions[i].x, explosions[i].y, 1);
            arduboy.drawPixel(explosions[i].x, explosions[i].y+1, 1);
            arduboy.drawPixel(explosions[i].x, explosions[i].y-1, 1);
            arduboy.drawPixel(explosions[i].x-1, explosions[i].y, 1);
            arduboy.drawPixel(explosions[i].x+1, explosions[i].y, 1);

            arduboy.drawPixel(explosions[i].x, explosions[i].y+2, 1);
            arduboy.drawPixel(explosions[i].x, explosions[i].y-2, 1);
            arduboy.drawPixel(explosions[i].x-2, explosions[i].y, 1);
            arduboy.drawPixel(explosions[i].x+2, explosions[i].y, 1);
            arduboy.drawPixel(explosions[i].x, explosions[i].y+3, 1);
            arduboy.drawPixel(explosions[i].x, explosions[i].y-3, 1);
            arduboy.drawPixel(explosions[i].x-3, explosions[i].y, 1);
            arduboy.drawPixel(explosions[i].x+3, explosions[i].y, 1);

            // diagonal lines
            arduboy.drawPixel(explosions[i].x+1, explosions[i].y+1, 1);
            arduboy.drawPixel(explosions[i].x+1, explosions[i].y-1, 1);
            arduboy.drawPixel(explosions[i].x-1, explosions[i].y+1, 1);
            arduboy.drawPixel(explosions[i].x-1, explosions[i].y-1, 1);
            arduboy.drawPixel(explosions[i].x+2, explosions[i].y+2, 1);
            arduboy.drawPixel(explosions[i].x+2, explosions[i].y-2, 1);
            arduboy.drawPixel(explosions[i].x-2, explosions[i].y+2, 1);
            arduboy.drawPixel(explosions[i].x-2, explosions[i].y-2, 1);

            explosions[i].tick++;
        } else if(explosions[i].tick < 60) {
            finished = false;
            arduboy.drawPixel(explosions[i].x, explosions[i].y, 1);
            arduboy.drawPixel(explosions[i].x, explosions[i].y+1, 1);
            arduboy.drawPixel(explosions[i].x, explosions[i].y-1, 1);
            arduboy.drawPixel(explosions[i].x+1, explosions[i].y, 1);
            arduboy.drawPixel(explosions[i].x-1, explosions[i].y, 1);

            arduboy.drawPixel(explosions[i].x, explosions[i].y+4, 1);
            arduboy.drawPixel(explosions[i].x, explosions[i].y-4, 1);
            arduboy.drawPixel(explosions[i].x+4, explosions[i].y, 1);
            arduboy.drawPixel(explosions[i].x-4, explosions[i].y, 1);
            arduboy.drawPixel(explosions[i].x, explosions[i].y+3, 1);
            arduboy.drawPixel(explosions[i].x, explosions[i].y-3, 1);
            arduboy.drawPixel(explosions[i].x+3, explosions[i].y, 1);
            arduboy.drawPixel(explosions[i].x-3, explosions[i].y, 1);

            // diagonal lines
            arduboy.drawPixel(explosions[i].x+3, explosions[i].y+3, 1);
            arduboy.drawPixel(explosions[i].x+3, explosions[i].y-3, 1);
            arduboy.drawPixel(explosions[i].x-3, explosions[i].y+3, 1);
            arduboy.drawPixel(explosions[i].x-3, explosions[i].y-3, 1);
            arduboy.drawPixel(explosions[i].x+2, explosions[i].y+2, 1);
            arduboy.drawPixel(explosions[i].x+2, explosions[i].y-2, 1);
            arduboy.drawPixel(explosions[i].x-2, explosions[i].y+2, 1);
            arduboy.drawPixel(explosions[i].x-2, explosions[i].y-2, 1);

            explosions[i].tick++;
        } else if(explosions[i].tick < 75) {
            finished = false;
            arduboy.drawPixel(explosions[i].x, explosions[i].y, 1);
            arduboy.drawPixel(explosions[i].x, explosions[i].y+1, 1);
            arduboy.drawPixel(explosions[i].x, explosions[i].y-1, 1);
            arduboy.drawPixel(explosions[i].x+1, explosions[i].y, 1);
            arduboy.drawPixel(explosions[i].x-1, explosions[i].y, 1);

            arduboy.drawPixel(explosions[i].x, explosions[i].y+4, 1);
            arduboy.drawPixel(explosions[i].x, explosions[i].y-4, 1);
            arduboy.drawPixel(explosions[i].x+4, explosions[i].y, 1);
            arduboy.drawPixel(explosions[i].x-4, explosions[i].y, 1);
            arduboy.drawPixel(explosions[i].x, explosions[i].y+5, 1);
            arduboy.drawPixel(explosions[i].x, explosions[i].y-5, 1);
            arduboy.drawPixel(explosions[i].x+5, explosions[i].y, 1);
            arduboy.drawPixel(explosions[i].x-5, explosions[i].y, 1);

            // diagonal lines
            arduboy.drawPixel(explosions[i].x+3, explosions[i].y+3, 1);
            arduboy.drawPixel(explosions[i].x+3, explosions[i].y-3, 1);
            arduboy.drawPixel(explosions[i].x-3, explosions[i].y+3, 1);
            arduboy.drawPixel(explosions[i].x-3, explosions[i].y-3, 1);
            arduboy.drawPixel(explosions[i].x+4, explosions[i].y+4, 1);
            arduboy.drawPixel(explosions[i].x+4, explosions[i].y-4, 1);
            arduboy.drawPixel(explosions[i].x-4, explosions[i].y+4, 1);
            arduboy.drawPixel(explosions[i].x-4, explosions[i].y-4, 1);

            explosions[i].tick++;
        }
    }
    return finished;
}

void drawScore() {
    arduboy.setCursor(70, 0);
    arduboy.print("Score:");
    arduboy.setCursor(110, 0);
    arduboy.print(player.score);
}

void drawLives() {
    // Draw multiple hearts.
    if(player.lives < 4) {
        for(byte i=0; i<player.lives; i++) {
            arduboy.drawPixel(i*7+2, 1, 1);
            arduboy.drawPixel(i*7+3, 1, 1);
            arduboy.drawPixel(i*7+1, 0, 1);
            arduboy.drawPixel(i*7+4, 0, 1);
            arduboy.drawPixel(i*7, 1, 1);
            arduboy.drawPixel(i*7+5, 1, 1);
            arduboy.drawPixel(i*7, 2, 1);
            arduboy.drawPixel(i*7+5, 2, 1);
            arduboy.drawPixel(i*7+1, 3, 1);
            arduboy.drawPixel(i*7+4, 3, 1);
            arduboy.drawPixel(i*7+2, 4, 1);
            arduboy.drawPixel(i*7+3, 4, 1);
        }
    } else {
        // Draw an heart.
        arduboy.drawPixel(2, 1, 1);
        arduboy.drawPixel(3, 1, 1);
        arduboy.drawPixel(1, 0, 1);
        arduboy.drawPixel(4, 0, 1);
        arduboy.drawPixel(0, 1, 1);
        arduboy.drawPixel(5, 1, 1);
        arduboy.drawPixel(0, 2, 1);
        arduboy.drawPixel(5, 2, 1);
        arduboy.drawPixel(1, 3, 1);
        arduboy.drawPixel(4, 3, 1);
        arduboy.drawPixel(2, 4, 1);
        arduboy.drawPixel(3, 4, 1);
        // Draw a cross.
        arduboy.drawPixel(7, 1, 1);
        arduboy.drawPixel(8, 2, 1);
        arduboy.drawPixel(9, 3, 1);
        arduboy.drawPixel(7, 3, 1);
        arduboy.drawPixel(9, 1, 1);
        // Get the number of lives.
        arduboy.setCursor(11, 0);
        arduboy.print(player.lives);
    }
}

void drawPause() {
    bool paused = true;
    //Draw pause to the screen
    arduboy.setCursor(52, 45);
    arduboy.print("PAUSE");
    arduboy.display();
    while(paused) {
        delay(200);
        if(arduboy.pressed(B_BUTTON)) {
            arduboy.fillRect(52, 45, 30, 11, 0);
            paused=false;
        }
    }
    delay(150);
}

void drawGameOver() {
    arduboy.setCursor(52, 42);
    arduboy.print( "Game");
    arduboy.setCursor(52, 54);
    arduboy.print("Over");
    arduboy.display();
}
