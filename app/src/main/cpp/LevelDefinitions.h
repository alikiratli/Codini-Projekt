#ifndef CODINI_LEVEL_DEFINITIONS_H
#define CODINI_LEVEL_DEFINITIONS_H

#include "Model.h"
#include <vector>
#include <string>

// Level-Schwierigkeiten
enum class Difficulty {
    EASY,      // Anfänger
    MEDIUM,    // Mittel
    HARD,      // Schwer
    EXPERT     // Experte
};

// Spezielle Level-Objekte
struct LevelObject {
    enum class Type {
        WALL,           // Wand
        BRIDGE,         // Brücke
        SWITCH,         // Schalter
        DOOR,          // Tür
        ITEM,          // Sammelbare Gegenstände
        TELEPORTER,    // Teleporter
        OBSTACLE       // Hindernis
    };

    Type type;
    Position position;
    bool isActive;     // Für Schalter, Türen
    int linkedId;      // Für verknüpfte Objekte (z.B. Schalter-Tür)
    std::string textureId;
};

// Level-Erfolgskriterien
struct LevelCriteria {
    int maxCommands;           // Maximale Anzahl von Befehlen
    float timeLimit;           // Zeitlimit (Sekunden)
    int minItemsCollected;     // Mindest-Sammelgegenstände
    bool requireOptimalPath;   // Ist optimaler Weg erforderlich
    std::vector<std::string> requiredCommands; // Erforderliche Befehle
};

// Level-Definition
struct LevelDefinition {
    int levelNumber;
    std::string name;
    std::string description;
    Difficulty difficulty;
    ThemeType theme;
    
    // Startzustand
    std::vector<Position> startPositions;  // Startpositionen der Boxen
    std::vector<Position> targetPositions; // Zielpositionen
    
    // Level-Eigenschaften
    std::vector<LevelObject> objects;      // Level-Objekte
    std::vector<CommandType> availableCommands; // Verfügbare Befehle
    LevelCriteria criteria;                // Erfolgskriterien
    
    // Tutorial-Tipps
    std::vector<std::string> tutorials;    // Level-Tipps
    std::vector<std::string> hints;        // Hinweise bei Blockierung
};

// Definition aller Level
class LevelDefinitions {
public:
    static std::vector<LevelDefinition> getAllLevels() {
        std::vector<LevelDefinition> levels;

        // Level 1: Grundbewegungen
        levels.push_back({
            1,
            "Erste Schritte",
            "Lerne die grundlegenden Bewegungsbefehle",
            Difficulty::EASY,
            ThemeType::ROBOT_LAB,
            {{1, 1}},           // Startposition
            {{4, 1}},           // Zielposition
            {},                 // Einfaches Level, keine Hindernisse
            {                   // Verfügbare Befehle
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

        // Level 2: Schleifen
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
                {"LOOP_START"} // Schleife muss verwendet werden
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

        // Weitere Level...
        return levels;
    }
};

#endif //CODINI_LEVEL_DEFINITIONS_H
