package com.example.codini

import android.os.Bundle
import android.view.MotionEvent
import android.view.View
import android.widget.Button
import android.widget.LinearLayout
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10
import com.example.codini.databinding.ActivityMainBinding
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class MainActivity : AppCompatActivity() {
    private lateinit var binding: ActivityMainBinding
    private val glSurfaceView get() = binding.glSurfaceView
    private val commandPanel get() = binding.commandPanel
    private val gameModel = GameModel()

    // Native methods (geçici devre dışı)
    // private external fun nativeInit()
    // private external fun nativeRender()
    // private external fun nativeOnSurfaceCreated()
    // private external fun nativeOnSurfaceChanged(width: Int, height: Int)
    // private external fun nativeOnTouch(x: Float, y: Float)

    companion object {
        // init {
        //     System.loadLibrary("codini")
        // }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

<<<<<<< HEAD
        // Initialize GameModel
        gameModel = GameModel()

        // glSurfaceView = findViewById(R.id.glSurfaceView)
        // commandPanel = findViewById(R.id.commandPanel)

        // glSurfaceView.setEGLContextClientVersion(3)
        // glSurfaceView.setRenderer(object : GLSurfaceView.Renderer {
        //     override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        //         // nativeOnSurfaceCreated()  // devre dışı bırakıldı
        //     }

        //     override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        //         // nativeOnSurfaceChanged(width, height)  // devre dışı bırakıldı
        //     }

        //     override fun onDrawFrame(gl: GL10?) {
        //         // nativeRender()  // devre dışı bırakıldı
        //     }
        // })

        // glSurfaceView.setOnTouchListener { _, event ->
        //     // tüm touch olayları native çağrı olmadan işleniyor
        //     true
        // }

        setupLoginUI()
        // setupGameUI()  // devre dışı bırakıldı, sadece login sonrası UI kurulacak

        // Zu Beginn Login-Bildschirm anzeigen
        showLoginScreen()
    }

    private fun setupGameUI() {
        // Spiel-UI-Setup wird hier durchgeführt (devre dışı)
        // initializeGame()
=======
        initRendering()
        setupLoginUI()
        showLoginScreen()
    }

    private fun initRendering() {
        glSurfaceView.setEGLContextClientVersion(3)
        glSurfaceView.setRenderer(object : GLSurfaceView.Renderer {
            override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) = nativeOnSurfaceCreated()

            override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) = nativeOnSurfaceChanged(width, height)

            override fun onDrawFrame(gl: GL10?) = nativeRender()
        })
        glSurfaceView.setOnTouchListener { _, event ->
            event.action == MotionEvent.ACTION_DOWN && nativeOnTouch(event.x, event.y).let { true }
        }
    }

    override fun onResume() {
        super.onResume()
        glSurfaceView.onResume()
    }

    override fun onPause() {
        glSurfaceView.onPause()
        super.onPause()
>>>>>>> bf23b1a (4. Commit)
    }

    private fun setupLoginUI() {
        binding.loginButton.setOnClickListener {
<<<<<<< HEAD
            val username = binding.usernameInput.text.toString()
            val password = binding.passwordInput.text.toString()
            if (gameModel.loginUser(username, password)) {
=======
            val (user, pass) = binding.usernameInput.text.toString() to binding.passwordInput.text.toString()
            if (gameModel.loginUser(user, pass)) {
>>>>>>> bf23b1a (4. Commit)
                showGameScreen()
                updateUserProgress()
            } else {
                showError(getString(R.string.login_error))
<<<<<<< HEAD
            }
        }
        binding.registerButton.setOnClickListener {
            val username = binding.usernameInput.text.toString()
            val password = binding.passwordInput.text.toString()
            if (gameModel.registerUser(username, password)) {
                showSuccess(getString(R.string.register_success))
            } else {
                showError(getString(R.string.register_error))
=======
>>>>>>> bf23b1a (4. Commit)
            }
        }
        binding.registerButton.setOnClickListener {
            val (user, pass) = binding.usernameInput.text.toString() to binding.passwordInput.text.toString()
            if (gameModel.registerUser(user, pass)) showSuccess(getString(R.string.register_success))
            else showError(getString(R.string.register_error))
        }
    }

    private fun updateUserProgress() {
        val progress = gameModel.getCurrentProgress()
        progress?.let {
            binding.levelText.text = getString(R.string.current_level, it.currentLevel)
            binding.scoreText.text = getString(R.string.total_score, it.totalScore)
<<<<<<< HEAD
            // Sterne anzeigen
            val currentLevelStars = it.levelStars[it.currentLevel - 1] ?: 0
            updateStars(currentLevelStars)
=======
            // Show stars based on progress
            val stars = it.levelStars[it.currentLevel - 1] ?: 0
            updateStars(stars)
>>>>>>> bf23b1a (4. Commit)
        }
    }

    private fun updateStars(starCount: Int) {
        binding.star1.setImageResource(if (starCount >= 1) R.drawable.star_filled else R.drawable.star_empty)
        binding.star2.setImageResource(if (starCount >= 2) R.drawable.star_filled else R.drawable.star_empty)
        binding.star3.setImageResource(if (starCount >= 3) R.drawable.star_filled else R.drawable.star_empty)
    }

    private fun showLoginScreen() {
        binding.loginLayout.visibility = View.VISIBLE
        binding.gameLayout.visibility = View.GONE
        binding.progressLayout.visibility = View.GONE
        glSurfaceView.onPause() // pause rendering when in login
    }

    private fun showGameScreen() {
        binding.loginLayout.visibility = View.GONE
        binding.gameLayout.visibility = View.VISIBLE
        binding.progressLayout.visibility = View.VISIBLE
<<<<<<< HEAD

        // Aktuelles Level laden
        val currentLevel = gameModel.getCurrentLevel()
        gameModel.initializeLevel(currentLevel)
=======
        glSurfaceView.onResume() // resume rendering
        // Initialize native game and load level
        initializeGame()
>>>>>>> bf23b1a (4. Commit)
    }

    private fun showSuccess(message: String) {
        Toast.makeText(this, message, Toast.LENGTH_SHORT).show()
    }

    private fun showError(message: String) {
        Toast.makeText(this, message, Toast.LENGTH_SHORT).show()
    }

    private fun initializeGame() {
        // nativeInit() devre dışı bırakıldı
        updateCommandPanel(listOf("vorwärts", "rückwärts", "rechts", "links"))
    }

    private fun updateCommandPanel(commands: List<String>) {
        commandPanel.removeAllViews()
        commands.forEach { command ->
            val button = Button(this)
            button.text = command
            button.setOnClickListener {
                // Wenn Befehl ausgewählt wird, dem nativen Code mitteilen
                executeCommand(command)
            }
            commandPanel.addView(button)
        }
    }

    private external fun executeCommand(command: String)
}