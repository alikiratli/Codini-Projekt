package com.example.codini

import android.os.Bundle
import android.view.MotionEvent
import android.view.View
import android.widget.Button
import android.widget.LinearLayout
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import android.opengl.GLSurfaceView
import com.example.codini.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {
    private lateinit var binding: ActivityMainBinding
    private lateinit var glSurfaceView: GLSurfaceView
    private lateinit var commandPanel: LinearLayout
    private lateinit var gameModel: GameModel

    private external fun nativeInit()
    private external fun nativeRender()
    private external fun nativeOnSurfaceCreated()
    private external fun nativeOnSurfaceChanged(width: Int, height: Int)
    private external fun nativeOnTouch(x: Float, y: Float)

    companion object {
        init {
            System.loadLibrary("codini")
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        glSurfaceView = findViewById(R.id.glSurfaceView)
        commandPanel = findViewById(R.id.commandPanel)

        glSurfaceView.setEGLContextClientVersion(3)
        glSurfaceView.setRenderer(object : GLSurfaceView.Renderer {
            override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
                nativeOnSurfaceCreated()
            }

            override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
                nativeOnSurfaceChanged(width, height)
            }

            override fun onDrawFrame(gl: GL10?) {
                nativeRender()
            }
        })

        glSurfaceView.setOnTouchListener { _, event ->
            when (event.action) {
                MotionEvent.ACTION_DOWN -> {
                    nativeOnTouch(event.x, event.y)
                    true
                }
                else -> false
            }
        }

        setupLoginUI()
        setupGameUI()

        // Başlangıçta login ekranını göster
        showLoginScreen()
    }

    private fun setupLoginUI() {
        binding.loginLayout.apply {
            loginButton.setOnClickListener {
                val username = usernameInput.text.toString()
                val password = passwordInput.text.toString()

                if (gameModel.loginUser(username, password)) {
                    showGameScreen()
                    updateUserProgress()
                } else {
                    showError(getString(R.string.login_error))
                }
            }

            registerButton.setOnClickListener {
                val username = usernameInput.text.toString()
                val password = passwordInput.text.toString()

                if (gameModel.registerUser(username, password)) {
                    showSuccess(getString(R.string.register_success))
                } else {
                    showError(getString(R.string.register_error))
                }
            }
        }
    }

    private fun updateUserProgress() {
        binding.progressLayout.apply {
            val progress = gameModel.getCurrentProgress()
            progress?.let {
                levelText.text = getString(R.string.current_level, it.currentLevel)
                scoreText.text = getString(R.string.total_score, it.totalScore)

                // Yıldızları göster
                val currentLevelStars = it.levelStars[it.currentLevel - 1] ?: 0
                updateStars(currentLevelStars)
            }
        }
    }

    private fun updateStars(starCount: Int) {
        binding.progressLayout.apply {
            star1.setImageResource(if (starCount >= 1) R.drawable.star_filled else R.drawable.star_empty)
            star2.setImageResource(if (starCount >= 2) R.drawable.star_filled else R.drawable.star_empty)
            star3.setImageResource(if (starCount >= 3) R.drawable.star_filled else R.drawable.star_empty)
        }
    }

    private fun showLoginScreen() {
        binding.loginLayout.visibility = View.VISIBLE
        binding.gameLayout.visibility = View.GONE
        binding.progressLayout.visibility = View.GONE
    }

    private fun showGameScreen() {
        binding.loginLayout.visibility = View.GONE
        binding.gameLayout.visibility = View.VISIBLE
        binding.progressLayout.visibility = View.VISIBLE

        // Mevcut seviyeyi yükle
        val currentLevel = gameModel.getCurrentLevel()
        gameModel.initializeLevel(currentLevel)
    }

    private fun showSuccess(message: String) {
        Toast.makeText(this, message, Toast.LENGTH_SHORT).show()
    }

    private fun showError(message: String) {
        Toast.makeText(this, message, Toast.LENGTH_SHORT).show()
    }

    private fun initializeGame() {
        nativeInit()
        updateCommandPanel(listOf("ileri", "geri", "sağa", "sola"))
    }

    private fun updateCommandPanel(commands: List<String>) {
        commandPanel.removeAllViews()
        commands.forEach { command ->
            val button = Button(this)
            button.text = command
            button.setOnClickListener {
                // Komut seçildiğinde native tarafa bildir
                executeCommand(command)
            }
            commandPanel.addView(button)
        }
    }

    private external fun executeCommand(command: String)
}