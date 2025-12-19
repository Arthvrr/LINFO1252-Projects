Exécuter le code :

1. Problème des Philosophes :

```bash
make philosoph N=x
```
(x étant le nombre de philosophes voulus)


2. Producer/Consumer

```bash
make prod_cons PROD=x CONS=y 
```
(x et y étant respectivement le nombre de threads du producer et consumer voulus) 


3. Lecteurs/Écrivains

```bash
make reader_writer WRITER=x READER=y 
```
(x et y étant respectivement le nombre de threads d'écrivains et de lecteurs voulus) 


4. Test de performance

```bash
cd src
cd perf
chmod +x test_perf.sh
./test_perf.sh
```


5. Test de performance du Test-and-set

```bash
cd src
cd perf
chmod +x perf_test_and_set.sh
./perf_test_and_set.sh
```


6. Test de performance du Test-and-test-and-set

```bash
cd src
cd perf
chmod +x perf_test_and_test_and_set.sh
./perf_test_and_test_and_set.sh
```


7. Problème des philosophes (test_and_set)
 
```bash
make test_and_set_philosoph N=x
```
(x étant le nombre de philosophes voulus)


8. Producer/Consumer (test_and_set)

```bash
make test_and_set_prod_cons PROD=x CONS=y
```
(x et y étant respectivement le nombre de threads du producer et consumer voulus) 


9. Lecteurs/Écrivains (test_and_set)

```bash
make test_and_set_reader_writer WRITER=x READER=y 
```
(x et y étant respectivement le nombre de threads d'écrivains et de lecteurs voulus) 


10. Problème des philosophes (test_and_test_and_set)

```bash
make test_and_test_and_set_philosoph N=x
```
(x étant le nombre de philosophes voulus)


11. Producer/Consumer (test_and_test_and_set)

```bash
make test_and_test_and_set_prod_cons PROD=x CONS=y
```
(x et y étant respectivement le nombre de threads du producer et consumer voulus) 


12. Lecteurs/Écrivains (test_and_test_and_set)

```bash
make test_and_test_and_set_reader_writer WRITER=x READER=y 
```
(x et y étant respectivement le nombre de threads d'écrivains et de lecteurs voulus) 


13. Test de performance des 3 problèmes Test-and-set

```bash
cd src
cd perf
chmod +x tas_perf.sh
./test_perf.sh
```


14. Test de performance des 3 problèmes Test-and-test-and-set

```bash
cd src
cd perf
chmod +x tas_perf.sh
./test_perf.sh
```

15. Test-and-set-main

```bash
make test_and_set_main N=x
```
(x étant le nombre de threads voulus) 


16. Test-and-test-and-set-main

```bash
make test_and_test_and_set_main N=x
```
(x étant le nombre de threads voulus) 
