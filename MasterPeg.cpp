#include "MasterPeg.h"
#include "Game.h"
#include "Sprites.h"

#include <QVector2D>

using namespace PGG;

/*int ciao = -45;*/
bool sortbysec(const std::tuple<int, int>& a,
    const std::tuple<int, int>& b)
{
    return (std::get<1>(a) < std::get<1>(b));
}

MasterPeg::MasterPeg(QPoint pos) : QGraphicsPixmapItem(0)
{
	setPos(pos);
	setPixmap(Sprites::instance()->get("master_peg").scaled(20, 20));
	Game::instance()->world()->addItem(this);

}


void MasterPeg::simulAdvance(b2Body* box) {
    if (box->GetPosition().y > 35)
    {
        QPointF center(720, 100);
        QLineF v2(center, QCursor::pos());
        v2.setLength(200.0);
        box->SetTransform(b2Vec2(v2.p2().x() / 30.0, v2.p2().y() / 30.0), box->GetAngle());
        box->SetLinearVelocity(b2Vec2(0, 0));
        box->SetAngularVelocity(0);
        Game::instance()->getWorld2d()->SetGravity(b2Vec2(0, 0));
        Game::instance()->alpha--;
        if (Game::instance()->alpha < -89) {
            Game::instance()->printScore();
            printf("Finitooo");

            Game::instance()->getBucketGraphic()->setPos(QPoint(0, Game::instance()->sceneRect().height()));
            emit Game::instance()->changeEngine();
            Game::instance()->getMasterPegGraphic()->setFire(false);
            Game::instance()->getBucketBox()->SetTransform(b2Vec2((Game::instance()->sceneRect().width() / 2) / 30.0, (Game::instance()->sceneRect().height() - 530) / 30.0), Game::instance()->getMasterPegBox()->GetAngle());

            if (Game::instance()->getSimulation()) {
                std::sort(Game::instance()->simulationScore.begin(), Game::instance()->simulationScore.end(), sortbysec);
                for (int i = 0; i < (Game::instance()->simulationScore.size()); i++) {
                    printf("Alpha: ");
                    printf("%d", std::get<0>(Game::instance()->simulationScore[i]));

                    printf(" Score: ");
                    printf("%d", std::get<1>(Game::instance()->simulationScore[i]));
                    printf("\n");
                }
                Game::instance()->getBucketBox()->SetTransform(b2Vec2((Game::instance()->sceneRect().width() / 2) / 30.0, (1080 - 50) / 30.0), Game::instance()->getBucketBox()->GetAngle());

                QPoint midPos((Game::instance()->sceneRect().width() / 2), 130);

                QLineF p = QLineF(midPos, QPointF((Game::instance()->sceneRect().width() / 2), 500));
                QLineF f = QLineF(midPos, QPointF((Game::instance()->sceneRect().width() / 2), 300));
                QLineF c = p;

                printf("%d", std::get<0>(Game::instance()->simulationScore[177]) + 1);

                p.setAngle((std::get<0>(Game::instance()->simulationScore[177])) + 1);
                f.setAngle((std::get<0>(Game::instance()->simulationScore[177])) + 1);
                QVector2D z = QVector2D(p.dx(), p.dy());
                z.normalize();

                Game::instance()->getCannon()->setTransformOriginPoint(QPoint(30, -65));
                Game::instance()->getCannon()->setRotation(-c.angleTo(p));
                if (!Game::instance()->getMasterPegGraphic()->getFire())
                    Game::instance()->getMasterPegBox()->SetTransform(b2Vec2(f.p2().x() / 30.0, f.p2().y() / 30.0), Game::instance()->getMasterPegBox()->GetAngle());
                Game::instance()->getMasterPegBox()->SetLinearVelocity(b2Vec2(z.x() * 15, z.y() * 15));
                Game::instance()->getMasterPegGraphic()->setFire(true);
                Game::instance()->getMasterPegGraphic()->setVisible(true);
                Game::instance()->getWorld2d()->SetGravity(b2Vec2(0, 25.0f));



            }
            Game::instance()->setSimulation(false);
        }
        else {
            printf("%d", Game::instance()->getSimulationScore());
            for (auto el : Game::instance()->PegBox) {
                static_cast<Peg*>(el->GetUserData())->setSimulHit(false);
            }
            Game::instance()->simulationScore.emplace_back(Game::instance()->alpha - 90, Game::instance()->getSimulationScore());

            Game::instance()->setSimulationScore(0);
            
            
            Game::instance()->fire(Game::instance()->alpha);


        }
    }
}

void MasterPeg::advance(b2Body* box) {

    this->setPos(box->GetPosition().x * 30.0, box->GetPosition().y * 30.0);

    if (box->GetPosition().y > 35)
    {


        down();
    }
            
      
    
}



void MasterPeg::down() {
    if (Game::instance()->getPower()) {
        switch (Game::instance()->getTurn() ? Game::instance()->getCharacter() : Game::instance()->getSecondCharacter()) {
        case Character::UNICORN:
        {
            Game::instance()->getCannon()->setPixmap(Sprites::instance()->get("cannon"));
            Game::instance()->getMasterPegGraphic()->setVisible(false);

            this->setFire(false);
            Game::instance()->clearHittedPeg();
            Game::instance()->setRemainingBall(Game::instance()->getRemainingBall() - 1);
            if ((Game::instance()->getRemainingBall() < 10) && (Game::instance()->getRemainingBall() >= 0))
                Game::instance()->lateral_mp[9 - (Game::instance()->getRemainingBall())]->setVisible(false);

            Game::instance()->printRemainingBall(Game::instance()->getRemainingBall());
            QPointF center(720, 100);


            QLineF v2(center, QCursor::pos());
            v2.setLength(200.0);
            Game::instance()->getMasterPegBox()->SetTransform(b2Vec2(v2.p2().x() / 30.0, v2.p2().y() / 30.0), Game::instance()->getMasterPegBox()->GetAngle());
            Game::instance()->getMasterPegBox()->SetLinearVelocity(b2Vec2(0, 0));
            Game::instance()->getMasterPegBox()->SetAngularVelocity(0);
            Game::instance()->getWorld2d()->SetGravity(b2Vec2(0, 0));
            Game::instance()->getBandOne()->setY(924);
            Game::instance()->getBandTwo()->setY(924);


            if (Game::instance()->getGameMode() == GameMode::CPU && !(Game::instance()->getTurn())) {
                if (!Game::instance()->getHardMode()) {
                    randomShot();
                }
                else {
                    Game::instance()->simulationScore.clear();
                    Game::instance()->setSimulationScore(0);
                    Game::instance()->alpha = 89;
                    Game::instance()->fire(Game::instance()->alpha);
                }

            }

            return;
        }
        break;
        case Character::BEAVER:
        {
            if (Game::instance()->getSecondMasterPegBox()->GetPosition().y > 35)
            {
                Game::instance()->getSecondMasterPegGraphics()->setVisible(false);
                Game::instance()->getSecondMasterPegBox()->DestroyFixture(Game::instance()->getSecondMasterPegBox()->GetFixtureList());
                Game::instance()->setPower(false);
            }
            return;
        }
        break;
        case Character::RABBIT:
        {
            if (Game::instance()->getSecondMasterPegBox()->GetPosition().y > 35)
            {
                Game::instance()->getSecondMasterPegGraphics()->setVisible(false);
                Game::instance()->getSecondMasterPegBox()->DestroyFixture(Game::instance()->getSecondMasterPegBox()->GetFixtureList());
                Game::instance()->setPower(false);
            }
            return;
        }
        break;
        case Character::PUMPKIN:
            Game::instance()->getMasterPegBox()->SetTransform(b2Vec2(Game::instance()->getMasterPegBox()->GetPosition().x, 0), Game::instance()->getMasterPegBox()->GetAngle());
            Game::instance()->setPower(false);
            return;
            break;
        case Character::DRAGON:
        {
            Game::instance()->getCannon()->setPixmap(Sprites::instance()->get("cannon_fired"));
            Game::instance()->getMasterPegGraphic()->setPixmap(Sprites::instance()->get("master_peg_fired").scaled(20, 20));
            Game::instance()->getMasterPegGraphic()->setVisible(false);
            Game::instance()->getMasterPegBox()->GetFixtureList()->SetSensor(true);

            Game::instance()->setPower(false);
            this->setFire(false);
            Game::instance()->clearHittedPeg();
            Game::instance()->setRemainingBall(Game::instance()->getRemainingBall() - 1);
            if ((Game::instance()->getRemainingBall() < 10) && (Game::instance()->getRemainingBall() >= 0))
                Game::instance()->lateral_mp[9 - (Game::instance()->getRemainingBall())]->setVisible(false);

            Game::instance()->printRemainingBall(Game::instance()->getRemainingBall());
            QPointF center(720, 100);


            QLineF v2(center, QCursor::pos());
            v2.setLength(200.0);
            Game::instance()->getMasterPegBox()->SetTransform(b2Vec2(v2.p2().x() / 30.0, v2.p2().y() / 30.0), Game::instance()->getMasterPegBox()->GetAngle());
            Game::instance()->getMasterPegBox()->SetLinearVelocity(b2Vec2(0, 0));
            Game::instance()->getMasterPegBox()->SetAngularVelocity(0);
            Game::instance()->getWorld2d()->SetGravity(b2Vec2(0, 0));
            Game::instance()->getBandOne()->setY(924);
            Game::instance()->getBandTwo()->setY(924);

            if (Game::instance()->getGameMode() == GameMode::CPU && !(Game::instance()->getTurn())){
                if (!Game::instance()->getHardMode()) {
                    randomShot();
                }
                else {
                    Game::instance()->simulationScore.clear();
                    Game::instance()->setSimulationScore(0);
                    Game::instance()->alpha = 89;
                    Game::instance()->fire(Game::instance()->alpha);
                }

            }

            return;
        }
        break;
        case Character::OWL:{
            Game::instance()->getCannon()->setPixmap(Sprites::instance()->get("cannon"));

            Game::instance()->getMasterPegGraphic()->setVisible(false);

            Game::instance()->setPower(false);
            this->setFire(false);
            Game::instance()->clearHittedPeg();
            Game::instance()->setRemainingBall(Game::instance()->getRemainingBall() - 1);
            if ((Game::instance()->getRemainingBall() < 10)&& (Game::instance()->getRemainingBall() >= 0))
                Game::instance()->lateral_mp[9 - (Game::instance()->getRemainingBall())]->setVisible(false);

            Game::instance()->printRemainingBall(Game::instance()->getRemainingBall());
            QPointF center(720, 100);


            QLineF v2(center, QCursor::pos());
            v2.setLength(200.0);
            Game::instance()->getMasterPegBox()->SetTransform(b2Vec2(v2.p2().x() / 30.0, v2.p2().y() / 30.0), Game::instance()->getMasterPegBox()->GetAngle());
            Game::instance()->getMasterPegBox()->SetLinearVelocity(b2Vec2(0, 0));
            Game::instance()->getMasterPegBox()->SetAngularVelocity(0);
            Game::instance()->getWorld2d()->SetGravity(b2Vec2(0, 0));
            Game::instance()->getBandOne()->setY(924);
            Game::instance()->getBandTwo()->setY(924);


            Game::instance()->alpha = 89;
            Game::instance()->simulationScore.clear();
            Game::instance()->setSimulationScore(0);
            Game::instance()->fire(Game::instance()->alpha);
            return;
        }
        break;
        }
    }
        if (Game::instance()->getGameMode() == GameMode::DUEL || Game::instance()->getGameMode() == GameMode::CPU) {
            if (Game::instance()->getTurn() == true) {
                Game::instance()->setTurn(false);
                switch (Game::instance()->getSecondCharacter()) {
                case Character::UNICORN:
                    Game::instance()->getCharacterFace()->setPixmap(QPixmap(Sprites::instance()->get("unicorn_face_right")));
                    break;
                case Character::BEAVER:
                    Game::instance()->getCharacterFace()->setPixmap(QPixmap(Sprites::instance()->get("beaver_face_right")));
                    break;
                case Character::CRAB:
                    Game::instance()->getCharacterFace()->setPixmap(QPixmap(Sprites::instance()->get("crab_face_right")));
                    break;
                case Character::FLOWER:
                    Game::instance()->getCharacterFace()->setPixmap(QPixmap(Sprites::instance()->get("flower_face_right")));
                    break;
                case Character::PUMPKIN:
                    Game::instance()->getCharacterFace()->setPixmap(QPixmap(Sprites::instance()->get("pumpkin_face_right")));
                    break;
                case Character::ALIEN:
                    Game::instance()->getCharacterFace()->setPixmap(QPixmap(Sprites::instance()->get("alien_face_right")));
                    break;
                case Character::OWL:
                    Game::instance()->getCharacterFace()->setPixmap(QPixmap(Sprites::instance()->get("owl_face_right")));
                    break;
                case Character::DRAGON:
                    Game::instance()->getCharacterFace()->setPixmap(QPixmap(Sprites::instance()->get("dragon_face_right")));
                    break;
                case Character::RABBIT:
                    Game::instance()->getCharacterFace()->setPixmap(QPixmap(Sprites::instance()->get("rabbit_face_right")));
                    break;
                }

            }
            else {
                Game::instance()->setTurn(true);
                switch (Game::instance()->getCharacter()) {
                case Character::UNICORN:
                    Game::instance()->getCharacterFace()->setPixmap(QPixmap(Sprites::instance()->get("unicorn_face_right")));
                    break;
                case Character::BEAVER:
                    Game::instance()->getCharacterFace()->setPixmap(QPixmap(Sprites::instance()->get("beaver_face_right")));
                    break;
                case Character::CRAB:
                    Game::instance()->getCharacterFace()->setPixmap(QPixmap(Sprites::instance()->get("crab_face_right")));
                    break;
                case Character::FLOWER:
                    Game::instance()->getCharacterFace()->setPixmap(QPixmap(Sprites::instance()->get("flower_face_right")));
                    break;
                case Character::PUMPKIN:
                    Game::instance()->getCharacterFace()->setPixmap(QPixmap(Sprites::instance()->get("pumpkin_face_right")));
                    break;
                case Character::ALIEN:
                    Game::instance()->getCharacterFace()->setPixmap(QPixmap(Sprites::instance()->get("alien_face_right")));
                    break;
                case Character::OWL:
                    Game::instance()->getCharacterFace()->setPixmap(QPixmap(Sprites::instance()->get("owl_face_right")));
                    break;
                case Character::DRAGON:
                    Game::instance()->getCharacterFace()->setPixmap(QPixmap(Sprites::instance()->get("dragon_face_right")));
                    break;
                case Character::RABBIT:
                    Game::instance()->getCharacterFace()->setPixmap(QPixmap(Sprites::instance()->get("rabbit_face_right")));
                    break;
                }
            }
        }

        Game::instance()->setPower(false);

            
        Game::instance()->getMasterPegGraphic()->setPixmap(Sprites::instance()->get("master_peg").scaled(20, 20));
        Game::instance()->getMasterPegBox()->GetFixtureList()->SetSensor(false);
        this->setFire(false);
        Game::instance()->clearHittedPeg();
        Game::instance()->setRemainingBall(Game::instance()->getRemainingBall() - 1);
        if ((Game::instance()->getRemainingBall() < 10) && (Game::instance()->getRemainingBall() >= 0))
            Game::instance()->lateral_mp[9 - (Game::instance()->getRemainingBall())]->setVisible(false);

        Game::instance()->printRemainingBall(Game::instance()->getRemainingBall());
        QPointF center(720, 100);


        QLineF v2(center, QCursor::pos());
        v2.setLength(200.0);
        Game::instance()->getMasterPegBox()->SetTransform(b2Vec2(v2.p2().x() / 30.0, v2.p2().y() / 30.0), Game::instance()->getMasterPegBox()->GetAngle());
        Game::instance()->getMasterPegBox()->SetLinearVelocity(b2Vec2(0, 0));
        Game::instance()->getMasterPegBox()->SetAngularVelocity(0);
        Game::instance()->getWorld2d()->SetGravity(b2Vec2(0, 0));
        Game::instance()->getBandOne()->setY(924);
        Game::instance()->getBandTwo()->setY(924);
        Game::instance()->getMasterPegGraphic()->setVisible(false);
        Game::instance()->getCannon()->setPixmap(Sprites::instance()->get("cannon"));

        if (Game::instance()->getRemainingBall() <= 0) {

            emit Game::instance()->gameOver();
            return;
        }

        if (Game::instance()->getGameMode() == GameMode::CPU && !(Game::instance()->getTurn())) {

            if (!Game::instance()->getHardMode()) {
                randomShot();
            }
            else
            {
                Game::instance()->simulationScore.clear();
                Game::instance()->alpha = 89;
                Game::instance()->setSimulationScore(0);
                Game::instance()->fire(Game::instance()->alpha);
            }
        }

    
}


void MasterPeg::randomShot() {
    int alfa = 20 + (rand() % 160);;
    QPoint midPos((Game::instance()->sceneRect().width() / 2), 130);

    QLineF p = QLineF(midPos, QPointF((Game::instance()->sceneRect().width() / 2), 500));
    QLineF f = QLineF(midPos, QPointF((Game::instance()->sceneRect().width() / 2), 300));
    QLineF c = p;

    p.setAngle(-alfa);
    f.setAngle(-alfa);

    Game::instance()->getCannon()->setTransformOriginPoint(QPoint(30, -65));
    Game::instance()->getCannon()->setRotation(-c.angleTo(p));
    if (!Game::instance()->getMasterPegGraphic()->getFire())
        Game::instance()->getMasterPegBox()->SetTransform(b2Vec2(f.p2().x() / 30.0, f.p2().y() / 30.0), Game::instance()->getMasterPegBox()->GetAngle());
    Game::instance()->getMasterPegBox()->SetLinearVelocity(b2Vec2((p.dx() - (Game::instance()->getMasterPegBox()->GetPosition().x / 30.0)) * 0.05, (p.dy() - (Game::instance()->getMasterPegBox()->GetPosition().y) / 30.0) * 0.05));
    Game::instance()->getWorld2d()->SetGravity(b2Vec2(0, 25.0f));
    Game::instance()->getMasterPegGraphic()->setFire(true);
    Game::instance()->getMasterPegGraphic()->setVisible(true);
    Game::instance()->getCannon()->setPixmap(Sprites::instance()->get("cannon_without_ball"));
}