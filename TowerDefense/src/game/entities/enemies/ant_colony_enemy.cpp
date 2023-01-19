#include "ant_colony_enemy.hpp"

#include "globals.hpp"
#include "red_ant_enemy.hpp"
#include "ant_enemy.hpp"

#define HEALTH_BAR_WIDTH 101.f
#define HEALTH_BAR_HEIGHT 25.f
#define HEALTH_BAR_HP_OFFSET_X 12.f
#define HEALTH_BAR_HP_OFFSET_Y 14.f
#define HEALTH_BAR_HP_HEIGHT 6.f

#define ANT_SPAWN_COOLDOWN_DELAY 0.2f
#define ANT_SPAWN_AROUND_COOLDOWN_DELAY 3.f
#define ANT_SPAWN_AROUND_COUNT_MIN 2
#define ANT_SPAWN_AROUND_COUNT_MAX 6
#define ANT_SPAWN_AROUND_COUNT_DEATH 10
#define ANT_SPAWN_AROUND_HEALTH_MULTIPLIER 0.5f
#define ANT_SPAWN_AROUND_SCALE 0.07f
#define ANT_SPAWN_AROUND_RED_CHANCE 0.4f
#define ANT_SPAWN_AROUND_POSITION_DELTA_X 70
#define ANT_SPAWN_AROUND_POSITION_DELTA_Y 20

// Not the best place
#define RANDOM_FLOAT(max) (((float_t)rand()/(float_t)(RAND_MAX)) * (max))

AntColonyEnemy::AntColonyEnemy(Point2 pos)
    : Enemy(pos, 10000, UINT16_MAX, 500),
    mHealthBar(Globals::gResources->GetTexture("ui\\boss_health_bar"))
{
	mTexture = Globals::gResources->GetTexture("enemies\\ant_colony");
	SetScale(.2f);
	mSpeed = 0.3f;
    mRotateForward = false;
    mAntSpawnAroundCooldown = ANT_SPAWN_AROUND_COOLDOWN_DELAY;
}

AntColonyEnemy::~AntColonyEnemy()
{
}

void AntColonyEnemy::OnUpdate()
{
    Enemy::OnUpdate();

    if (mAntSpawnAroundCooldown <= 0.f)
    {
        SpawnAntsAround(ANT_SPAWN_AROUND_COUNT_MIN + rand() % (ANT_SPAWN_AROUND_COUNT_MAX - ANT_SPAWN_AROUND_COUNT_MIN));
        // +- 1/4 of the delay is random
        mAntSpawnAroundCooldown = ANT_SPAWN_AROUND_COOLDOWN_DELAY + ANT_SPAWN_AROUND_COOLDOWN_DELAY / 2.f - RANDOM_FLOAT(ANT_SPAWN_AROUND_COOLDOWN_DELAY / 4);
    }

    if (mAntSpawnCooldown <= 0.f)
    {
        SpawnAnt();
        // +- 1/4 of the delay is random
        mAntSpawnCooldown = ANT_SPAWN_COOLDOWN_DELAY;
    }

    if (mHealth <= 0)
        SpawnAntsAround(ANT_SPAWN_AROUND_COUNT_DEATH);

    mAntSpawnAroundCooldown -= Globals::gGame->GetPlayingSpeedDeltaTime();
    mAntSpawnCooldown -= Globals::gGame->GetPlayingSpeedDeltaTime();
}

void AntColonyEnemy::SpawnAntsAround(uint8_t count)
{
    for (uint8_t i = 0; i < count; i++)
    {
        bool redAnt = RANDOM_FLOAT(1) <= ANT_SPAWN_AROUND_RED_CHANCE;

        Point2 antPos(GetPixelPosition().x + ANT_SPAWN_AROUND_POSITION_DELTA_X / 2 - rand() % ANT_SPAWN_AROUND_POSITION_DELTA_X,
            GetPixelPosition().y + ANT_SPAWN_AROUND_POSITION_DELTA_Y / 2 - rand() % ANT_SPAWN_AROUND_POSITION_DELTA_Y);

        Enemy* ant = nullptr;
        if (redAnt)
            ant = new RedAntEnemy(antPos);
        else
            ant = new AntEnemy(antPos);
        ant->SetCurrentPathIndex(mCurrentPathIndex);
        ant->SetScale(ANT_SPAWN_AROUND_SCALE);
        ant->SetSpawnHealth(ant->GetSpawnHealth() * ANT_SPAWN_AROUND_HEALTH_MULTIPLIER);
        ant->SetHealth(ant->GetHealth() * ANT_SPAWN_AROUND_HEALTH_MULTIPLIER);
        ant->SetMoneyDrop(1);

        Globals::gGame->enemiesQueue.push_back(ant);
    }
}

void AntColonyEnemy::SpawnAnt()
{
    int32_t screenX;
    int32_t screenY;
    Point2 position;

    PlayField* pf = Globals::gGame->GetPlayField();
    uint8_t tileX = pf->GetPathNodes()[0].x;
    uint8_t tileY = pf->GetPathNodes()[0].y;

    pf->GetPixelPositionFromGrid(tileX, tileY, screenX, screenY);
    Point2 pos(screenX, screenY + GRID_SQUARE_SIZE / 2);

    Enemy* ant = new AntEnemy(pos);
    ant->SetMoneyDrop(1);
    Globals::gGame->enemiesQueue.push_back(ant);
}

void AntColonyEnemy::DrawHealthBar(ImVec2 &pos)
{
	pos.y -= 70.f;
	pos.x -= 51.f;
    ImVec2 bottomRight(pos.x + HEALTH_BAR_WIDTH, pos.y + HEALTH_BAR_HEIGHT);
    Globals::gDrawList->AddImage(mHealthBar->id, pos, bottomRight);
    pos.x += HEALTH_BAR_HP_OFFSET_X;
    pos.y += HEALTH_BAR_HP_OFFSET_Y;
	Globals::gDrawList->AddRectFilled(pos, ImVec2(pos.x + ((HEALTH_BAR_WIDTH - HEALTH_BAR_HP_OFFSET_X * 2) * mHealth / mSpawnHealth), pos.y + HEALTH_BAR_HP_HEIGHT), IM_COL32(0xA0, 0, 0, 0xFF), 3.f);
}

#undef RANDOM_FLOAT
