Les sous dossiers:

Bruit : un bruit est créé directement par le code et son onde oppose et la somme sont générés.

Sons : Quelques exemples de sons téléchargés (bruit de ventilation, chauffe à gaz, …)

Simulation : simulation avec les bruit téléchargés, génération de l’onde opposées et de la somme résultante

Real time : pas tout à fait du temps réel pour l’instant, on enregistre avec l’ordi pendant 8secondes un bruit quelconque et ensuite générer l’onde opposées et de la somme résultante


nrf_code : contient les codes à pousser sur la carte
-Penser à build en précisant :
	-board target : "nrf5340_audio_dk/nrf5340/cpuapp
	-Base configuration file : prj.conf
	-Base devicetree overlays : "nrf5340_audio_dk.overlay"