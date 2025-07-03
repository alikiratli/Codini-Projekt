package com.example.codini

<<<<<<< HEAD
import java.util.*

data class UserProgress(
    var currentLevel: Int = 1,
    var totalScore: Int = 0,
    val levelScores: MutableMap<Int, Int> = mutableMapOf(),
    val levelStars: MutableMap<Int, Int> = mutableMapOf(),
    var lastPlayTime: Long = System.currentTimeMillis()
)

data class UserProfile(
    val username: String,
    var passwordHash: String,
    var progress: UserProgress = UserProgress(),
    var isLoggedIn: Boolean = false
)

data class LevelCompletion(
    val score: Int,
    val stars: Int,
    val commandsUsed: Int,
    val timeSpent: Float,
    val isOptimalSolution: Boolean
)

class GameModel {
    init {
        // Test kullanıcısı oluşturulur
        registerUser("user", "parola")
    }
    private val users = mutableMapOf<String, UserProfile>()
    private var currentUser: UserProfile? = null
    
    fun loginUser(username: String, password: String): Boolean {
        val hashedPassword = hashPassword(password)
        val user = users[username]
        return if (user != null && user.passwordHash == hashedPassword) {
            currentUser = user.copy(isLoggedIn = true)
            users[username] = currentUser!!
            true
        } else {
            false
        }
    }
    
    fun registerUser(username: String, password: String): Boolean {
        return if (!users.containsKey(username)) {
            val newUser = UserProfile(
                username = username,
                passwordHash = hashPassword(password),
                isLoggedIn = false
            )
            users[username] = newUser
            true
        } else {
            false
        }
    }
    
    fun getCurrentProgress(): UserProgress? = currentUser?.progress
    
    fun getCurrentLevel(): Int = currentUser?.progress?.currentLevel ?: 1
    
    fun initializeLevel(level: Int) {
        // Diese Methode wird eine Verbindung zum nativen C++ Code herstellen
        // Momentan leere Implementierung
    }
    
    private fun hashPassword(password: String): String {
        // Einfache Hash-Implementierung - sollte in Produktion sicherer sein
        return password.hashCode().toString()
    }
    
    fun checkWinCondition(): Boolean {
        // Diese Methode wird vom nativen Code implementiert
        return false
    }
    
    fun getCommands(): List<String> {
        // Verfügbare Befehle für das aktuelle Level
        return when (getCurrentLevel()) {
            1 -> listOf("vorwärts", "rechts", "links")
            2 -> listOf("vorwärts", "rechts", "links", "wiederholen")
            3 -> listOf("vorwärts", "rechts", "links", "wiederholen", "schleife")
            else -> listOf("vorwärts", "rückwärts", "rechts", "links", "springen", "teleportieren")
        }
=======
data class UserProgress(
    val currentLevel: Int,
    val totalScore: Int,
    val levelStars: Map<Int, Int>
)

class GameModel {
    fun loginUser(username: String, password: String): Boolean {
        // TODO: Implement actual login logic
        return true
    }

    fun registerUser(username: String, password: String): Boolean {
        // TODO: Implement actual registration logic
        return true
    }

    fun getCurrentLevel(): Int {
        // TODO: Return actual current level
        return 1
    }

    fun initializeLevel(level: Int) {
        // TODO: Initialize level data
    }

    fun getCurrentProgress(): UserProgress? {
        return UserProgress(currentLevel = 1, totalScore = 0, levelStars = mapOf(1 to 0))
>>>>>>> bf23b1a (4. Commit)
    }
}
