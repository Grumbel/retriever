               _____________________  ________   _____________
              /  __ \  __/______/__ \/  /  __/| / /  ___/  __ \
             /    __/ /_  /  /    __/  /  /_| |/ /  /_ /    __/
            /  /  \  /__ /  /  /  \/  /  /_ | / /  /__/  /  \  
           /__/ \__\___//__/__/ \__\_/_____/|__/_____/__/ \__\ 
           ======================  V0.1  =====================

In dieser Datei sollten alle wichtigen Informationen stehen die f�r das Spiel
notwendig sind, es ist aber nicht garantiert, da� diese Datei immer auf dem
neusten Stand ist, da die Entwicklung im Augenblick ziemlich chaotisch
ablaeuft und es ist nicht sicherzustellen, da� alle Dateien immer auf dem
selben Stand sind.


Introduction:
~~~~~~~~~~~~~
Retriever ist als Adventure Spiel, �hnlich den LucasArts Adventures, geplant,
vielleicht wird es noch um weitere Dinge erweitert, aber das steht noch nicht
fest, erstmal mu� die Adventure Engine laufen ;)
Das ganze Spiel, inklusive Quellcode, steht zur freien Verf�gung und ich werde
nicht versuchen irgendwelchen Profit daraus zu ziehen. Jede Art von
Verbreitung ist erlaubt und erw�nscht.
Die Bedienung sollte relative leicht fallen, mit der rechten Maustaste ruft
man ein kleines Menue auf aus dem man dann eine Funktion aus w�hlt (anschauen,
benutzen, nehmen, Inventar zeigen). Die Funktion wird dann auf das im Augenblick
anvisiert Objekt ausgef�hrt. Mit der linken Maustaste kann man sich herum
bewegen und die R�ume wechseln. Im Augenblick sollte man noch nichts an R�tsel
erwarten, es gibt also nichts zu schafen und wer den Abspann sehen will der
sollte einfach F11 dr�cken. Wem beim herumst�bern irgendwelche Ideen kommen
f�r R�tsel, der sollte mir <grumbel@gmx.de> das am besten mitteilen und ich
werde versuchen es einzubauen.
Wem sonst noch irgendwelches zu diesem Spiel einf�llt, egal ob Lob, zus�tzliche
Ideen oder vernichtende Kritik, der kann mir das gerne mitteilen, einfach eine
Mail an mich und ich werd sehen was ich tun kann.


WWW:
~~~~
Die aktuellste Version sollte immer unter http://home.pages.de/~retriever/
zur Verf�gung stehen.


Installation:
~~~~~~~~~~~~~
Alle Zip-Dateien in  ein beliebiges Verzeichnis installieren, darauf achten,
da� die Verzeichnisstruktur erhalten bleibt (z.B. bei mit PKZip "-d" entpacken).


Spiel starten:
~~~~~~~~~~~~~~
Gebe einfach START.BAT ein um das Spiel zu starten.
M�gliche Parameter sind:

  -?              zeigt Hilfetext an
  -nosound        kein Sound
  -nointro        das Intro wird �bersprungen, sehr n�tzlich, da das
                  Intro sp�testens nach dem dritten mal nervt
  -showroom X     zeigt die Informationen zu dem Raum X an
  -startroom X    laest das Spiel im Raum X beginnen
  -midi [File]    abspielen eines beliebigen Midi-Files
  -novsync        sort f�r eine schnelle ungebremsten Spielverlauf
  -rdf [File]]    [File] statt RETRIEV.RDF benutzen

Falls das Spiel direkt unter MS-DOS gestartet werden soll, dann ist
noch ein DPMI-Server notwendig. Ein solcher ist zum Beispiel CWSDPMI von
Charles W Sandmann <sandmann@clio.rice.edu>. Er kann z.B. unter
ftp://ftp.uni-paderborn.de/pub/simtelnet/gnu/djgpp/v2misc/csdpmi3b.zip
heruntergeladen werden und sollte im selben Verzeichnis liegen wie
RETRIEV.EXE. Wer Quake auf seiner Festplatte hat, hat �brigens CSDPMI schon
auf seiner Festplatte und sollte es vom Quake Verzeichnis ins Retriever 
Verzeichnis kopieren.


Programmierung:
~~~~~~~~~~~~~~~
Zum compilieren des Source-Code sollte ein einfaches MAKE.EXE im Verzeichnis
des Source-Codes ausreichen.
Wenn interesse besteht an dem Spiel mit zu programmieren, dann am besten
eine Mail an mich <grumbel@gmx.de>. 


Struktur von Retriever [nicht mehr aktuell, sorry]:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Der Quell-Code besteht momentan aus f�nf Dateien (beschreibung siehe unten). 
Das Spiel selbst besteht im wesentlichen aus dem Objekt GUY1 und aus einem
Array von ROOM Objekten. GUY dient dazu den Sprite zu bewegen und zu animieren.
ROOM beinhaltet die Daten und Funktionen f�r die einzelnen R�ume.


Dateien [nicht mehr aktuell, sorry]:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Main-File:
      RETRIEV.EXE -nointro -nosound -showroom<int>

   Projekt-Files:
      RETRIEV.GDT   RETRIEV.GPR 
      "RHIDE.EXE RETRIEV" sollte das Projekt laden.
      (Verzeichnisse m�ssen angepast werden! OPTIONS/DIRECTORIES/...)

   Source-Files:
      MAIN.CC     - Hauptprogramm
      FUNCTION.H  - allgemeine Funktionen
      FUNC.CC     - Standart Funktionen
      PROTOTYP.H  - s�mtliche Prototypen f�r FUNC.CC
      OBJECTS.H   - hier werden die externen Variablen definiert

   Sound-Files:
      RAINMAN.MID - Hintergrundmusik
      MAKELEV.WAV - Ger�usch beim benutzen des Gateways (nicht integriert!)

   Daten-Files:
      RETRIEV.RDF - Objektdaten f�r das Spiel

   Grafik-Dateien:
      1.PCX        - Startraum
      1_MAP.PCX    - Map f�r Startraum
      2TUER_1.PCX  - T�r f�r Startraum
      2TUER_2.PCX  - T�r f�r Startraum
      2TUER_3.PCX  - T�r f�r Startraum
      3.PCX        - Outside
      3_MAP.PCX    - Map f�r Outside
      4.PCX        - Outside2
      4_MAP.PCX    - Map f�r Outside2         (not in use)
      ABGRUND.PCX  - Abgrund (Sicht von oben) (not in use)
      ABGRUND2.PCX - Abgrund                  (not in use)
      BAUM.PCX     - Baum f�r Startraum
      CUBE.PCX     - W�rfel                   (not in use)
      GRUMBEL.PCX  - Firmen Logo
      GUY.PCX      - verschiedene Animationsphasen von Guy1 (not in use)
      GUY1.PCX     - verschiedene Animationsphasen von Guy1 (not in use)
      GUY2.PCX     - verschiedene Animationsphasen von Guy1 (not in use)
      GUY3.PCX     - verschiedene Animationsphasen von Guy1 (not in use)
      GUY4.PCX     - verschiedene Animationsphasen von Guy1 (not in use)
      GUY5.PCX     - verschiedene Animationsphasen von Guy1 (not in use)
      MOGLI_01.PCX - verschiedene Animationsphasen von Guy1
      .
      .
      .
      MOGLI_24.PCX - verschiedene Animationsphasen von Guy1

      ODD.PCX      - schwebender W�rfel f�r Startraum
      PFLANZE.PCX  - Pflanze f�r Outside
      PFLANZE4.PCX - Pflanze f�r Outside2
      REPLIC.PCX   - unbekanntes Objekt       (not in use)
      RETRIE2.PCX  - alternatives Titelbild
      RETRIEV.PCX  - Titelbild
      TUER_1.PCX   - T�r f�r Outside
      TUER_2.PCX   - T�r f�r Outside
      TUER_3.PCX   - T�r f�r Outside


RDF-Fileformat [nicht mehr aktuell, sorry]:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   #<Int> beginnt einen neuen Raum und beendet den letzten
   PCX-Datei f�r den Hintergrund
   Map-Datei f�r den Raum
   Objektname
   PCX-Datei f�r Objekt
   Beschreiben f�r Objekt beim anschauen
   X Y Z Koordinaten f�r Objektgrafik Position
   X1 Y1 X2 Y2 Koordinaten f�r Objektgr��e (Mousekolision)
   X Y DIRECTION Koordinaten f�r WALK_TO
   Objekttyp (0 - normal; 1 - T�r) Objektdaten (Raumnummer des n�chsten Raumes)
   .
   .
   .

   es k�nnen noch weitere Objekte oder ein neuer Raum folgen


Dank an:
~~~~~~~~~~
  DJ Delorie f�r DJGPP
  Shawn Hargreaves f�r Allegro
  und allen Schreibern der NewsGroup comp.os.ms-dos.djgpp
  sowie den Schreibern der Allegro Mailling List.

-- 
(c) 1997
Ingo Ruhnke, <grumbel@gmx.de>
http://home.pages.de/~grumbel/