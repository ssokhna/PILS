import numpy as np
import matplotlib.pyplot as plt
import soundfile as sf
import os

# Vérifier si le fichier existe
if os.path.exists('../sons/chauffe-eau-a-gaz.wav'):
    # Charger un fichier audio
    data, fs = sf.read('../sons/chauffe-eau-a-gaz.wav')
    print(f"Fichier audio chargé, durée : {len(data)/fs} secondes")
    
    # Générer le signal opposé
    opposite_data = -data
    
    # Sauvegarder le résultat
    sf.write('opposite_audio.wav', opposite_data, fs)
    print("Fichier audio inversé sauvegardé sous 'output_audio.wav'.")
else:
    print("Le fichier est introuvable.")

# Calculer la somme des deux signaux
combined_data = data + opposite_data
sf.write('summed_audio.wav', combined_data, fs)
print("Fichier 'summed_audio.wav' (somme des signaux) généré.")

# Calculer le temps pour la visualisation (en fonction de la fréquence d'échantillonnage)
t = np.arange(0, len(data)) / fs

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
plt.savefig('audio_signals_plot.png') 
plt.show()

# Résultat
print("Les fichiers audio suivants ont été générés :")
print("- 'input_audio.wav' : Signal original")
print("- 'inverted_audio.wav' : Signal inversé")
print("- 'summed_audio.wav' : Somme des signaux")