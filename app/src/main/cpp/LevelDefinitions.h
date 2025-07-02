#ifndef CODINI_LEVEL_DEFINITIONS_H
#define CODINI_LEVEL_DEFINITIONS_H

#include "Model.h"
#include <vector>
#include <string>

// Seviye zorlukları
enum class Difficulty {
    EASY,      // Başlangıç
    MEDIUM,    // Orta
    HARD,      // Zor
    EXPERT     // Uzman
};

// Özel seviye nesneleri
struct LevelObject {
    enum class Type {
        WALL,           // Duvar
        BRIDGE,         // Köprü
        SWITCH,         // Düğme
        DOOR,          // Kapı
        ITEM,          // Toplanabilir eşya
        TELEPORTER,    // Işınlayıcı
        OBSTACLE       // Engel
    };

    Type type;
    Position position;
    bool isActive;     // Düğmeler, kapılar için
    int linkedId;      // Bağlantılı nesneler için (örn: düğme-kapı)
    std::string textureId;
};

// Seviye başarı koşulları
struct LevelCriteria {
    int maxCommands;           // Maksimum komut sayısı
    float timeLimit;           // Zaman sınırı (saniye)
    int minItemsCollected;     // Minimum toplanması gereken eşya
    bool requireOptimalPath;   // En kısa yol gerekli mi
    std::vector<std::string> requiredCommands; // Kullanılması gereken komutlar
};

// Seviye tanımı
struct LevelDefinition {
    int levelNumber;
    std::string name;
    std::string description;
    Difficulty difficulty;
    ThemeType theme;
    
    // Başlangıç durumu
    std::vector<Position> startPositions;  // Kutuların başlangıç pozisyonları
    std::vector<Position> targetPositions; // Hedef pozisyonları
    
    // Level özellikleri
    std::vector<LevelObject> objects;      // Seviye nesneleri
    std::vector<CommandType> availableCommands; // Kullanılabilir komutlar
    LevelCriteria criteria;                // Başarı kriterleri
    
    // Öğretici ipuçları
    std::vector<std::string> tutorials;    // Seviye ipuçları
    std::vector<std::string> hints;        // Takılınca gösterilecek ipuçları
};

// Tüm seviyelerin tanımları
class LevelDefinitions {
public:
    static std::vector<LevelDefinition> getAllLevels() {
        std::vector<LevelDefinition> levels;

        // Seviye 1: Temel Hareketler
        levels.push_back({
            1,
            "Erste Schritte",
            "Lerne die grundlegenden Bewegungsbefehle",
            Difficulty::EASY,
            ThemeType::ROBOT_LAB,
            {{1, 1}},           // Başlangıç pozisyonu
            {{4, 1}},           // Hedef pozisyonu
            {},                 // Basit seviye, engel yok
            {                   // Kullanılabilir komutlar
                CommandType::MOVE_FORWARD,
                CommandType::TURN_LEFT,
                CommandType::TURN_RIGHT
            },
            {                   // Başarı kriterleri
                5,             // Max 5 komut
                60.0f,         // 60 saniye
                0,             // Eşya yok
                true,          // Optimal yol gerekli
                {"MOVE_FORWARD"}
            },
            {                   // Öğretici mesajları
                "Willkommen beim Programmieren!",
                "Benutze die Bewegungsbefehle um den Roboter zum Ziel zu bringen."
            },
            {                   // İpuçları
                "Versuche zuerst vorwärts zu gehen",
                "Du kannst den Roboter auch drehen"
            }
        });

        // Seviye 2: Döngüler
        levels.push_back({
            2,
            "Wiederholungen",
            "Entdecke die Kraft der Schleifen",
            Difficulty::EASY,
            ThemeType::SPACE,
            {{1, 1}},           // Başlangıç
            {{1, 5}},           // Hedef
            {                   // Nesneler
                {LevelObject::Type::WALL, {2, 1}, false, -1, "wall"},
                {LevelObject::Type::WALL, {2, 2}, false, -1, "wall"},
                {LevelObject::Type::WALL, {2, 3}, false, -1, "wall"}
            },
            {                   // Komutlar
                CommandType::MOVE_FORWARD,
                CommandType::TURN_LEFT,
                CommandType::TURN_RIGHT,
                CommandType::LOOP_START,
                CommandType::LOOP_END
            },
            {                   // Kriterler
                8,             // Max 8 komut
                90.0f,         // 90 saniye
                0,             // Eşya yok
                true,          // Optimal yol
                {"LOOP_START"} // Döngü kullanılmalı
            },
            {                   // Öğretici
                "Schleifen helfen dir, Befehle zu wiederholen",
                "Benutze eine Schleife um den Code kürzer zu machen"
            },
            {                   // İpuçları
                "Zähle wie oft du die gleichen Befehle benutzt",
                "Eine Schleife kann Befehle mehrmals ausführen"
            }
        });

        // Diğer seviyeler...
        return levels;
    }
};

#endif //CODINI_LEVEL_DEFINITIONS_H
