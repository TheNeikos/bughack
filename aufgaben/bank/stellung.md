# Bank
## Szenario

Die Anwendung stellt ein Demo-Programm da, welches eine Account-Sperre hat, das
heißt, dass viele Funktionen erst nach einer Legitimierung zur Verfügung
stehen. So ein Programm kann beispielsweise in einer Bank am Bankautomaten, am
Schalter oder sogar für den Zugang des Filialleiters verwendet werden. Daher
ist für dieses Programm relevant, dass keine unbefugten Zugriffe möglich sind,
die beispielsweise der Hersteller der Software hätte implementieren können.

## Aufgabenstellung

Überprüfen Sie daher die Anwendung auf Möglichkeiten, die neben dem normalen
Login, einen weiteren Zutritt ins System gewähren könnten. Erläutern Sie Ihre
Vorgehensweise und dokumentieren Sie diese. Überprüfen Sie die Anwendung aber
auch auf weitere Schwachstellen im Bezug auf das Account-Management. Woher weiß
die Anwendung, welche Kennwörter richtig sind? Welche Benutzerrollen könnte es
geben? Und wo, vor allem aber wie sicher (beispielsweise Klartext, base64, MD5,
…), werden diese Informationen gespeichert?
