#include "Sprites.h"
#include <QBitmap>
#include <iostream>
#include <QPainter>

using namespace PGG;

// load texture with transparency using the given color as mask
static QPixmap loadTexture(const std::string &file, QColor mask_color = Qt::magenta)
{
    QPixmap pixmap(file.c_str());
    pixmap.setMask(pixmap.createMaskFromColor(mask_color));
    return pixmap;
}

// moving within sprite utility function //ci servirà per i font credo
QRect moveBy(QRect rect, int x, int y, int dx = 16, int dy = 16, int border_x = 0, int border_y = 0)
{
    rect.moveTo(QPoint(rect.x() + x * dx + x * border_x, rect.y() + y * dy + y * border_y));
    return rect;
}

// main object positions within sprites //non ha senso da togliere perché non ci serve
static QRect player(467, 259, 93, 93);
static QRect pegle(0, 0, 200, 200);
static QRect bucket(168, 200, 168, 24);
static QRect remainingBallNumber(479, 305, 30, 54);
static QRect score(4, 280, 46,70);

Sprites *Sprites::instance()
{
    static Sprites uniqueInstance;
    return &uniqueInstance;
}

Sprites::Sprites() //load in RAM
{
    title_screen = loadTexture(":/sprites/peggle_title.png", QColor(255, 0, 255));

    Hud_Unicorn = loadTexture(":/sprites/krita.png"); // da fare prob. nell'altra classe Hud
    //Hud_Drake ...

    //dynamic Hud da fare

    stage_elements = loadTexture(":/sprites/stage_elements.png", QColor(255, 0, 255));
    peggles = loadTexture(":/sprites/peggles.png", QColor(255, 255, 255));
    fonts = loadTexture(":/sprites/Peggle_Fonts.png");
    sprites = loadTexture(":/sprites/sprites.png");
}

QPixmap Sprites::get(const std::string &id)
{
    if (id == "peggle_title")
        return title_screen;

    else if (id == "Hud_Unicorn")
        return Hud_Unicorn;

    else if (id == "player")
        return sprites.copy(403, 82, 9, 12);
    else if (id == "bucket")
        return stage_elements.copy(bucket);

    // Remaining Balls
    else if (id == "0")
        return fonts.copy(remainingBallNumber);
    else if (id == "1")
        return fonts.copy(moveBy(remainingBallNumber, 1, 0, 30 * 1));
    else if (id == "2")
        return fonts.copy(moveBy(remainingBallNumber, 1, 0, (30 * 2)));
    else if (id == "3")
        return fonts.copy(moveBy(remainingBallNumber, 1, 0, (30 * 3)));
    else if (id == "4")
        return fonts.copy(moveBy(remainingBallNumber, 1, 0, (30 * 4)));
    else if (id == "5")
        return fonts.copy(moveBy(remainingBallNumber, 1, 0, (30 * 5)));
    else if (id == "6")
        return fonts.copy(moveBy(remainingBallNumber, 1, 0, (30 * 6)));
    else if (id == "7")
        return fonts.copy(moveBy(remainingBallNumber, 1, 0, (35 * 6)));
    else if (id == "8")
        return fonts.copy(moveBy(remainingBallNumber, 1, 0, (35 * 7)));
    else if (id == "9")
        return fonts.copy(moveBy(remainingBallNumber, 1, 0, (35 * 8)));
    else if (id == "10")
        return (fonts.copy(478, 305, 52, 54).transformed(QTransform().scale(-1, 1)));

    else if (id == "cannon")
        return sprites.copy(385, 390, 51, 64);


    else if (id == "peg_blue_hit")
        return peggles.copy(pegle);
    else if (id == "peg_red_hit")
        return peggles.copy(moveBy(pegle, 1, 0, 200));
    else if (id == "peg_blue")
        return peggles.copy(moveBy(pegle, 0, 1, 0, 200));
    else if (id == "peg_red")
        return peggles.copy(moveBy(pegle, 1, 1, 200, 200));


    else if (id == "band")
        return sprites.copy(91, 528, 15, 422);
    else if (id == "molt")
        return sprites.copy(535, 355, 50, 16);
    else if (id == "molt-x2")
        return sprites.copy(673, 324, 25, 20);
    else if (id == "molt-x3")
        return sprites.copy(673, 344, 25, 20);
    else if (id == "molt-x5")
        return sprites.copy(673, 364, 25, 20);
    else if (id == "molt-x10")
        return sprites.copy(673, 384, 25, 20);
    else
    {
        std::cerr << "Cannot find sprite texture with id \"" << id << "\"\n";
        return QPixmap();
    }
}

QPixmap Sprites::getNumber(int n, int fill)
{
    std::string text = std::to_string(n);

    // fill with 0s on the left
    if (fill)
        while (text.size() != fill)
            text = '0' + text;

    // create collage texture
    QPixmap collage(8 * int(text.size()), 8);
    QPainter painter(&collage);

    // add numbers
    for (int i = 0; i < text.size(); i++)
        painter.drawPixmap(8 * i, 0, Sprites::instance()->get(std::string("number-") + text[i]));

    // end painting (necessary for setMask)
    painter.end();

    // make background transparent
    collage.setMask(collage.createMaskFromColor(QColor(147, 187, 236)));

    return collage;
}

QPixmap Sprites::getString(std::string text, int fill)
{
    // fill with 0s on the left
    if (fill)
        while (text.size() != fill)
            text = ' ' + text;

    // create collage texture
    QPixmap collage(8 * int(text.size()), 8);
    QPainter painter(&collage);

    // add letters
    for (int i = 0; i < text.size(); i++)
        if (text[i] != ' ')
            painter.drawPixmap(8 * i, 0, Sprites::instance()->get(std::string("char-") + text[i]));

    // end painting (necessary for setMask)
    painter.end();

    // make background transparent
    collage.setMask(collage.createMaskFromColor(QColor(147, 187, 236)));

    return collage;
}

// score composite from sprite single score pieces
QPixmap Sprites::getScore(int s)
{
    bool thousands = s >= 1000;
    int base_score = s / (thousands ? 100 : 10);

    QPixmap collage(16, 8);
    QPainter painter(&collage);

    // add 0s
    

    // add base number
    //if (base_score == 10)
        painter.drawPixmap(0, 0, fonts.copy(score));
   /* else if (base_score == 20)
        painter.drawPixmap(0, 0, _stage_tiles.copy(moveBy(score, 1, 0, 8, 8)));
    else if (base_score == 40)
        painter.drawPixmap(0, 0, _stage_tiles.copy(moveBy(score, 2, 0, 8, 8)));
    else if (base_score == 50)
        painter.drawPixmap(0, 0, _stage_tiles.copy(moveBy(score, 3, 0, 8, 8)));
    else if (base_score == 80)
        painter.drawPixmap(0, 0, _stage_tiles.copy(moveBy(score, 4, 0, 8, 8)));
    else // 1 up
        painter.drawPixmap(0, 0, _stage_tiles.copy(QRect(718, 185, 16, 8)));
        */
    // end painting (necessary for setMask)
    painter.end();

    // make background transparent
    collage.setMask(collage.createMaskFromColor(QColor(147, 187, 236)));

    return collage;
}