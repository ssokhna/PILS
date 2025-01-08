import numpy as np
import matplotlib.pyplot as plt
import soundfile as sf

# Paramètres
fs = 44100  # Fréquence d'échantillonnage (44,1 kHz)
duration = 2  # Durée du signal en secondes
freq = 440  # Fréquence du signal (en Hz)

# Générer un signal sinusoïdal
t = np.linspace(0, duration, int(fs * duration), endpoint=False)
signal = np.sin(2 * np.pi * freq * t)

# Sauvegarder le signal simulé dans un fichier audio
sf.write('input_audio.wav', signal, fs)
print("Fichier 'input_audio.wav' généré.")

# Lire le fichier audio généré
data, fs = sf.read('input_audio.wav')

# Générer le signal inversé
opposite_data = -data
sf.write('inverted_audio.wav', opposite_data, fs)
print("Fichier 'inverted_audio.wav' (signal inversé) généré.")

# Calculer la somme des deux signaux
combined_data = data + opposite_data
sf.write('summed_audio.wav', combined_data, fs)
print("Fichier 'summed_audio.wav' (somme des signaux) généré.")

# Visualiser les signaux
plt.figure(figsize=(10, 6))

# Signal original
plt.subplot(3, 1, 1)
plt.plot(t[:1000], data[:1000], label="Signal original")
plt.title("Signal Original")
plt.xlabel("Temps (s)")
plt.ylabel("Amplitude")
plt.grid()

# Signal inversé
plt.subplot(3, 1, 2)
plt.plot(t[:1000], opposite_data[:1000], label="Signal inversé", color='orange')
plt.title("Signal Inversé")
plt.xlabel("Temps (s)")
plt.ylabel("Amplitude")
plt.grid()

# Somme des deux signaux
plt.subplot(3, 1, 3)
plt.plot(t[:1000], combined_data[:1000], label="Somme des deux signaux", color='green')
plt.title("Somme des Signaux")
plt.xlabel("Temps (s)")
plt.ylabel("Amplitude")
plt.grid()

plt.tight_layout()
plt.show()

# Résultat
print("Les fichiers audio suivants ont été générés :")
print("- 'input_audio.wav' : Signal original")
print("- 'inverted_audio.wav' : Signal inversé")
print("- 'summed_audio.wav' : Somme des signaux")
