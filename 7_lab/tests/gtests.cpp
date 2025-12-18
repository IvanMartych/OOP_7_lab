#include <gtest/gtest.h>
#include "../include/arena.h"
#include "../include/factory.h"
#include "../include/combat_visitor.h"
#include "../include/console_observer.h"
#include "../include/file_observer.h"
#include "../include/npc.h"
#include "../include/knight.h"
#include "../include/pegasus.h"
#include "../include/squirrel.h"
#include <memory>
#include <fstream>
#include <thread>
#include <chrono>



TEST(Lab7_NpcCreationTest, CreateKnight) {
    Knight knight(100, 100, "TestKnight");
    
    EXPECT_EQ(knight.getType(), "Knight");
    EXPECT_EQ(knight.getName(), "TestKnight");
    EXPECT_EQ(knight.getX(), 100);
    EXPECT_EQ(knight.getY(), 100);
    EXPECT_TRUE(knight.isAlive());
}

TEST(Lab7_NpcCreationTest, CreateSquirrel) {
    Squirrel squirrel(50, 75, "TestSquirrel");
    
    EXPECT_EQ(squirrel.getType(), "Squirrel");
    EXPECT_EQ(squirrel.getName(), "TestSquirrel");
    EXPECT_EQ(squirrel.getX(), 50);
    EXPECT_EQ(squirrel.getY(), 75);
    EXPECT_TRUE(squirrel.isAlive());
}

TEST(Lab7_NpcCreationTest, CreatePegasus) {
    Pegasus pegasus(80, 90, "TestPegasus");
    
    EXPECT_EQ(pegasus.getType(), "Pegasus");
    EXPECT_EQ(pegasus.getName(), "TestPegasus");
    EXPECT_EQ(pegasus.getX(), 80);
    EXPECT_EQ(pegasus.getY(), 90);
    EXPECT_TRUE(pegasus.isAlive());
}

// ==================== ТЕСТЫ ПАРАМЕТРОВ NPC (7 ЛАБА) ====================

TEST(Lab7_NpcParametersTest, KnightParameters) {
    Knight knight(0, 0, "Knight");
    
    // Рыцарь: расстояние хода 30, расстояние убийства 10
    EXPECT_EQ(knight.getMoveDistance(), 30);
    EXPECT_EQ(knight.getKillDistance(), 10);
}

TEST(Lab7_NpcParametersTest, SquirrelParameters) {
    Squirrel squirrel(0, 0, "Squirrel");
    
    // Белка: расстояние хода 5, расстояние убийства 5
    EXPECT_EQ(squirrel.getMoveDistance(), 5);
    EXPECT_EQ(squirrel.getKillDistance(), 5);
}

TEST(Lab7_NpcParametersTest, PegasusParameters) {
    Pegasus pegasus(0, 0, "Pegasus");
    
    // Пегас: расстояние хода 30, расстояние убийства 10
    EXPECT_EQ(pegasus.getMoveDistance(), 30);
    EXPECT_EQ(pegasus.getKillDistance(), 10);
}

// ==================== ТЕСТЫ СОСТОЯНИЯ ЖИЗНИ ====================

TEST(Lab7_NpcLifeTest, IsAliveByDefault) {
    Knight knight(50, 50, "Knight");
    EXPECT_TRUE(knight.isAlive());
}

TEST(Lab7_NpcLifeTest, KillNpc) {
    Squirrel squirrel(50, 50, "Squirrel");
    EXPECT_TRUE(squirrel.isAlive());
    
    squirrel.kill();
    EXPECT_FALSE(squirrel.isAlive());
}

TEST(Lab7_NpcLifeTest, MultipleKills) {
    Pegasus pegasus(50, 50, "Pegasus");
    
    pegasus.kill();
    EXPECT_FALSE(pegasus.isAlive());
    
    // Повторный kill не должен вызывать проблем
    pegasus.kill();
    EXPECT_FALSE(pegasus.isAlive());
}

// ==================== ТЕСТЫ ДВИЖЕНИЯ ====================

TEST(Lab7_NpcMovementTest, SetPosition) {
    Knight knight(10, 20, "Knight");
    
    knight.setPosition(50, 60);
    EXPECT_EQ(knight.getX(), 50);
    EXPECT_EQ(knight.getY(), 60);
}

TEST(Lab7_NpcMovementTest, MoveRandomStayInBounds) {
    Knight knight(50, 50, "Knight");
    
    // Двигаем несколько раз
    for (int i = 0; i < 20; ++i) {
        knight.moveRandom(100, 100);
        
        // Проверяем границы
        EXPECT_GE(knight.getX(), 0);
        EXPECT_LE(knight.getX(), 100);
        EXPECT_GE(knight.getY(), 0);
        EXPECT_LE(knight.getY(), 100);
    }
}

TEST(Lab7_NpcMovementTest, DeadNpcDoesNotMove) {
    Knight knight(50, 50, "Knight");
    knight.kill();
    
    int x_before = knight.getX();
    int y_before = knight.getY();
    
    knight.moveRandom(100, 100);
    
    // Мертвый NPC не двигается
    EXPECT_EQ(knight.getX(), x_before);
    EXPECT_EQ(knight.getY(), y_before);
}

TEST(Lab7_NpcMovementTest, SquirrelMovesLess) {
    Squirrel squirrel(50, 50, "Squirrel");
    
    // Белка двигается максимум на 5 единиц
    for (int i = 0; i < 10; ++i) {
        int x_before = squirrel.getX();
        int y_before = squirrel.getY();
        
        squirrel.moveRandom(100, 100);
        
        int dx = abs(squirrel.getX() - x_before);
        int dy = abs(squirrel.getY() - y_before);
        
        EXPECT_LE(dx, 5);
        EXPECT_LE(dy, 5);
    }
}

// ==================== ТЕСТЫ РАССТОЯНИЙ ====================

TEST(Lab7_NpcDistanceTest, DistanceCalculation) {
    Knight k1(0, 0, "K1");
    Knight k2(3, 4, "K2");
    
    // Расстояние по теореме Пифагора: sqrt(3^2 + 4^2) = 5
    EXPECT_DOUBLE_EQ(k1.distanceTo(k2), 5.0);
}

TEST(Lab7_NpcDistanceTest, DistanceSamePosition) {
    Squirrel s1(50, 50, "S1");
    Squirrel s2(50, 50, "S2");
    
    EXPECT_DOUBLE_EQ(s1.distanceTo(s2), 0.0);
}

TEST(Lab7_NpcDistanceTest, DistanceSymmetric) {
    Knight knight(10, 20, "Knight");
    Pegasus pegasus(50, 80, "Pegasus");
    
    EXPECT_DOUBLE_EQ(knight.distanceTo(pegasus), pegasus.distanceTo(knight));
}

// ==================== ТЕСТЫ ФАБРИКИ ====================

TEST(Lab7_FactoryTest, CreateAllTypes) {
    auto knight = NpcFactory::createNpc("Knight", "K1", 10, 20);
    auto squirrel = NpcFactory::createNpc("Squirrel", "S1", 30, 40);
    auto pegasus = NpcFactory::createNpc("Pegasus", "P1", 50, 60);
    
    ASSERT_NE(knight, nullptr);
    ASSERT_NE(squirrel, nullptr);
    ASSERT_NE(pegasus, nullptr);
    
    EXPECT_EQ(knight->getType(), "Knight");
    EXPECT_EQ(squirrel->getType(), "Squirrel");
    EXPECT_EQ(pegasus->getType(), "Pegasus");
}

TEST(Lab7_FactoryTest, CreateFromString) {
    auto knight = NpcFactory::createFromString("Knight TestKnight 100 200");
    
    ASSERT_NE(knight, nullptr);
    EXPECT_EQ(knight->getType(), "Knight");
    EXPECT_EQ(knight->getName(), "TestKnight");
    EXPECT_EQ(knight->getX(), 100);
    EXPECT_EQ(knight->getY(), 200);
}

TEST(Lab7_FactoryTest, InvalidTypeThrows) {
    EXPECT_THROW({
        auto npc = NpcFactory::createNpc("Dragon", "D1", 10, 10);
    }, std::invalid_argument);
}

// ==================== ТЕСТЫ АРЕНЫ ====================

TEST(Lab7_ArenaTest, CreateArena) {
    Arena arena(100, 100);
    EXPECT_EQ(arena.getNpcCount(), 0);
}

TEST(Lab7_ArenaTest, AddNpc) {
    Arena arena(100, 100);
    arena.addNpc(NpcFactory::createNpc("Knight", "K1", 50, 50));
    
    EXPECT_EQ(arena.getNpcCount(), 1);
}

TEST(Lab7_ArenaTest, AddMultipleNpcs) {
    Arena arena(100, 100);
    
    arena.addNpc(NpcFactory::createNpc("Knight", "K1", 10, 10));
    arena.addNpc(NpcFactory::createNpc("Squirrel", "S1", 20, 20));
    arena.addNpc(NpcFactory::createNpc("Pegasus", "P1", 30, 30));
    
    EXPECT_EQ(arena.getNpcCount(), 3);
}

TEST(Lab7_ArenaTest, GenerateRandomNpcs) {
    Arena arena(100, 100);
    
    arena.generateRandomNpcs(20);
    
    // Должно создаться примерно 20 NPC (могут быть коллизии имен)
    EXPECT_GE(arena.getNpcCount(), 15);
    EXPECT_LE(arena.getNpcCount(), 20);
}

TEST(Lab7_ArenaTest, ClearArena) {
    Arena arena(100, 100);
    arena.generateRandomNpcs(10);
    
    EXPECT_GT(arena.getNpcCount(), 0);
    
    arena.clear();
    EXPECT_EQ(arena.getNpcCount(), 0);
}

// ==================== ТЕСТЫ ВЫЖИВШИХ ====================

TEST(Lab7_ArenaTest, GetSurvivorsInitial) {
    Arena arena(100, 100);
    arena.generateRandomNpcs(10);
    
    auto survivors = arena.getSurvivors();
    
    // Все должны быть живы в начале
    EXPECT_EQ(survivors.size(), arena.getNpcCount());
    
    for (const auto* npc : survivors) {
        EXPECT_TRUE(npc->isAlive());
    }
}

TEST(Lab7_ArenaTest, GetSurvivorsAfterBattle) {
    Arena arena(100, 100);
    
    // Создаем NPC близко друг к другу
    arena.addNpc(NpcFactory::createNpc("Knight", "K1", 50, 50));
    arena.addNpc(NpcFactory::createNpc("Squirrel", "S1", 51, 51));
    arena.addNpc(NpcFactory::createNpc("Pegasus", "P1", 52, 52));
    
    EXPECT_EQ(arena.getNpcCount(), 3);
    
    // Бой с дальностью 10
    arena.startBattle(10.0);
    
    auto survivors = arena.getSurvivors();
    
    // Должно остаться меньше NPC
    EXPECT_LT(survivors.size(), 3);
    
    // Все выжившие живы
    for (const auto* npc : survivors) {
        EXPECT_TRUE(npc->isAlive());
    }
}

// ==================== ТЕСТЫ БОЕВОЙ СИСТЕМЫ ====================

TEST(Lab7_CombatTest, KnightKillsSquirrel) {
    CombatVisitor visitor;
    auto knight = NpcFactory::createNpc("Knight", "K1", 0, 0);
    auto squirrel = NpcFactory::createNpc("Squirrel", "S1", 5, 5);
    
    EXPECT_TRUE(visitor.canKill(knight.get(), squirrel.get()));
    EXPECT_FALSE(visitor.canKill(squirrel.get(), knight.get()));
}

TEST(Lab7_CombatTest, SquirrelKillsPegasus) {
    CombatVisitor visitor;
    auto squirrel = NpcFactory::createNpc("Squirrel", "S1", 0, 0);
    auto pegasus = NpcFactory::createNpc("Pegasus", "P1", 5, 5);
    
    EXPECT_TRUE(visitor.canKill(squirrel.get(), pegasus.get()));
    EXPECT_FALSE(visitor.canKill(pegasus.get(), squirrel.get()));
}

TEST(Lab7_CombatTest, PegasusDoesNotAttack) {
    CombatVisitor visitor;
    auto pegasus = NpcFactory::createNpc("Pegasus", "P1", 0, 0);
    auto knight = NpcFactory::createNpc("Knight", "K1", 5, 5);
    auto squirrel = NpcFactory::createNpc("Squirrel", "S1", 10, 10);
    
    // Пегас никого не атакует
    EXPECT_FALSE(visitor.canKill(pegasus.get(), knight.get()));
    EXPECT_FALSE(visitor.canKill(pegasus.get(), squirrel.get()));
    EXPECT_FALSE(visitor.canKill(pegasus.get(), pegasus.get()));
}

TEST(Lab7_CombatTest, KnightsDoNotFight) {
    CombatVisitor visitor;
    auto k1 = NpcFactory::createNpc("Knight", "K1", 0, 0);
    auto k2 = NpcFactory::createNpc("Knight", "K2", 5, 5);
    
    EXPECT_FALSE(visitor.canKill(k1.get(), k2.get()));
    EXPECT_FALSE(visitor.canKill(k2.get(), k1.get()));
}

TEST(Lab7_CombatTest, BattleOutOfRange) {
    Arena arena(100, 100);
    
    // NPC далеко друг от друга
    arena.addNpc(NpcFactory::createNpc("Knight", "K1", 10, 10));
    arena.addNpc(NpcFactory::createNpc("Squirrel", "S1", 90, 90));
    
    EXPECT_EQ(arena.getNpcCount(), 2);
    
    // Бой с малой дальностью
    arena.startBattle(20.0);
    
    // Оба должны выжить (слишком далеко)
    EXPECT_EQ(arena.getNpcCount(), 2);
}

// ==================== ТЕСТЫ МНОГОПОТОЧНОСТИ ====================

TEST(Lab7_MultithreadTest, StartGameShortDuration) {
    Arena arena(100, 100);
    arena.generateRandomNpcs(10);
    
    EXPECT_NO_THROW({
        arena.startGame(2);
    });
}

TEST(Lab7_MultithreadTest, GameReducesNpcCount) {
    Arena arena(100, 100);
    arena.generateRandomNpcs(20);
    
    size_t initialCount = arena.getNpcCount();
    
    arena.startGame(3);
    
    auto survivors = arena.getSurvivors();
    
    // После боев обычно становится меньше NPC
    EXPECT_LE(survivors.size(), initialCount);
}

TEST(Lab7_MultithreadTest, AllSurvivorsAreAlive) {
    Arena arena(100, 100);
    arena.generateRandomNpcs(15);
    
    arena.startGame(2);
    
    auto survivors = arena.getSurvivors();
    
    // Проверяем что все выжившие действительно живы
    for (const auto* npc : survivors) {
        EXPECT_TRUE(npc->isAlive());
    }
}

TEST(Lab7_MultithreadTest, ThreadSafety) {
    Arena arena(100, 100);
    arena.generateRandomNpcs(20);
    
    // Запускаем параллельные потоки, читающие выживших
    std::thread t1([&arena]() {
        for (int i = 0; i < 10; ++i) {
            auto survivors = arena.getSurvivors();
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    });
    
    std::thread t2([&arena]() {
        for (int i = 0; i < 10; ++i) {
            auto survivors = arena.getSurvivors();
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    });
    
    t1.join();
    t2.join();
    
    // Если нет deadlock - тест пройден
    SUCCEED();
}

// ==================== ТЕСТЫ НАБЛЮДАТЕЛЕЙ ====================

TEST(Lab7_ObserverTest, AddConsoleObserver) {
    Arena arena(100, 100);
    auto observer = std::make_shared<ConsoleObserver>();
    
    EXPECT_NO_THROW({
        arena.addObserver(observer);
    });
}

TEST(Lab7_ObserverTest, AddFileObserver) {
    Arena arena(100, 100);
    auto observer = std::make_shared<FileObserver>("test_log.txt");
    
    EXPECT_NO_THROW({
        arena.addObserver(observer);
    });
    
    std::remove("test_log.txt");
}

// ==================== ТЕСТЫ ПЕЧАТИ КАРТЫ ====================

TEST(Lab7_MapTest, PrintMapDoesNotThrow) {
    Arena arena(100, 100);
    arena.generateRandomNpcs(10);
    
    EXPECT_NO_THROW({
        arena.printMap();
    });
}

TEST(Lab7_MapTest, PrintEmptyMap) {
    Arena arena(100, 100);
    
    EXPECT_NO_THROW({
        arena.printMap();
    });
}

// ==================== ИНТЕГРАЦИОННЫЙ ТЕСТ ====================

TEST(Lab7_IntegrationTest, FullGameScenario) {
    Arena arena(100, 100);
    auto consoleObserver = std::make_shared<ConsoleObserver>();
    auto fileObserver = std::make_shared<FileObserver>("integration_log.txt");
    
    arena.addObserver(consoleObserver);
    arena.addObserver(fileObserver);
    
    arena.generateRandomNpcs(30);
    size_t initialCount = arena.getNpcCount();
    
    arena.startGame(5);
    
    auto survivors = arena.getSurvivors();
    
    EXPECT_GE(survivors.size(), 1);
    EXPECT_LE(survivors.size(), initialCount);
    
    for (const auto* npc : survivors) {
        EXPECT_TRUE(npc->isAlive());
    }
    
    std::remove("integration_log.txt");
}

// ==================== ТЕСТЫ СОХРАНЕНИЯ/ЗАГРУЗКИ ====================

TEST(Lab7_FileTest, SaveAndLoad) {
    std::string filename = "test_save_load.txt";
    
    {
        Arena arena(100, 100);
        arena.addNpc(NpcFactory::createNpc("Knight", "K1", 10, 20));
        arena.addNpc(NpcFactory::createNpc("Squirrel", "S1", 30, 40));
        arena.addNpc(NpcFactory::createNpc("Pegasus", "P1", 50, 60));
        
        arena.saveToFile(filename);
        EXPECT_EQ(arena.getNpcCount(), 3);
    }
    
    {
        Arena arena(100, 100);
        arena.loadFromFile(filename);
        
        EXPECT_EQ(arena.getNpcCount(), 3);
    }
    
    std::remove(filename.c_str());
}
