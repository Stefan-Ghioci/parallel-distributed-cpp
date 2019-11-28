# **PPD: Laborator 0**

## **Analiza cerințelor**

### **Cerințe**

Set de functii de ajutor in realizarea laboratoarelor:

1. Creare de fisier care contine numere intregi aleatoare dintr-un interval  precizat (parametrii: file_name; size; min; max)
2. Verificare daca doua fisiere contin acelasi date  (pentru date de tip: intreg, real); necesitate
    1. verificarea corectitudinii
3. Scriere in fisier Excel prin adaugare pe o anumita linie (sau doar adaugare  linie); necesitate
    1. evaluare performanta
    2. adaugare automata a timpului de  executie pentru rulari multiple
4. Script pentru executia automata a mai multor rulari a aceluiasi program

## **Proiectare**

``void write_data_to_csv_file(const string& filename, const vector\<string\>& data)``

``void generate_big_data_file(const string& filename, int size, int min, int max)``

``bool file_contents_equal(const string& filename1, const string& filename2)``
