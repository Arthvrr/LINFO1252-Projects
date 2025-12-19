Exécuter les tests :
```bash
make test
```

Créer une archive de soumission :
```bash
make submit
```

Mettre à jour l'archive TAR : 
```bash
tar -rf test_archive.tar test_directory/nom_fichier
```

Décompresser l'archive TAR : 
```bash
tar -xf test_archive.tar
```

Supprimer des fichiers de l'archive TAR : 
```bash
rm test_directory/nom_fichier
```

Vérifier le contenu de l'archive TAR :
```bash
tar -tf test_archive.tar
```

Recréer l'archive TAR :
```bash
tar -cf test_archive.tar test_directory/
```

