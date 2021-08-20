/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.cpp
 * Author: abhin
 *
 * Created on 6 June, 2020, 7:53 PM
 */

#include<iostream>
#include<cstdio>
#include<math.h>
#include<cstring>
#include<stdlib.h>
#include<fstream>
#include<cstdio>
#include<time.h>
using namespace std;
long int p, q, n, t, flag, e[100], d[100], temp[100], j, m[100], en[100],
i, enckey, deckey;
char msg[100];
int prime(long int);
void ce();
long int cd(long int);
void encrypt();
void decrypt();

int prime(long int pr) {//fn returns 1 if input is prime
    int i;
    j = sqrt(pr);
    for (i = 2; i <= j; i++) {
        if (pr % i == 0)
            return 0;
    }
    return 1;
}

int primegenerator(int low, int high) {
    int primearr[1000], i, j = 0, flag = 0, secret;
    srand(time(0)); //using current time as seed
    while (low < high) {
        flag = 0;
        for (i = 2; i <= low / 2; ++i) {
            if (low % i == 0) {
                flag = 1;
                break;
            }
        }
        if (flag == 0) {
            primearr[j] = low;
            ++j;
        }
        ++low;
    }


    secret = rand() % (j); //give out a random int between 0 and j

    return (primearr[secret]);
}

int main() {
    int choice;
    cout << "\nLevel of Encryption";
    cout << "\n1.Mobile/Portable";
    cout << "\n2.Intermediate";
    cout << "\n3.Crucial\n";
    cout << "\nEnter your choice:";
    cin>>choice;
    switch (choice) {
        case 1:
            p = primegenerator(63, 126);
            q = primegenerator(65, 152);
            break;
        case 2:
            p = primegenerator(75, 200);
            q = primegenerator(73, 205);
            break;
        case 3:
            p = primegenerator(124, 263);
            q = primegenerator(139, 230);
            break;
        default:
            cout << "Wrong Choice";
            exit(1);
    }

    cout << "\nENTER MESSAGE\n";
    fflush(stdin);
    gets(msg);
    for (i = 0; msg[i] != '\0'; i++)
        m[i] = msg[i];
    n = p * q;
    t = (p - 1) * (q - 1); // t = phi
    ce(); // gives a set of public keys and their private keys

    fstream f("public.dat", ios::out | ios::binary);
    fstream f1("private.dat", ios::out | ios::binary);

    srand(time(0)); // uses current time as seed
    int ee = rand() % 20;

    enckey = e[ee];
    deckey = d[ee];

    f << enckey << n;
    f1 << deckey << n;

    f.close();
    f1.close();
    cout << "\nPOSSIBLE VALUES OF e AND d ARE\n";
    for (i = 0; i < j - 1; i++)
        cout << e[i] << "\t" << d[i] << "\n";

    encrypt();
    decrypt();
    return 0;
}

void ce() {
    int k;
    k = 0;
    for (i = 2; i < t; i++) {
        if (t % i == 0) //filtering out the possible co-primes of i to extract e(public key)
            continue;
        flag = prime(i); //if co-prime flag turns to 1
        if (flag == 1 && i != p && i != q) {
            e[k] = i; //appending all the possible values of e(public key)
            flag = cd(e[k]); //calculating possible d(private key) for all e(public key)
            if (flag > 0) {
                d[k] = flag; //storing private keys to d(set of private keys)
                k++;
            }
            if (k == 99)
                break;
        }
    }
}

long int cd(long int x) {// calculating d by *d = (k * phi(n) + 1) / e* k = 1,2,3....
    long int k = 1;
    while (1) {
        k = k + t;
        if (k % x == 0)
            return (k / x);
    }
}

void encrypt() {
    ofstream f;
    f.open("enc.dat", ios::out | ios::binary); //file for encrypted data

    long int pt, ct, key = e[4], k, len;
    i = 0;
    len = strlen(msg);
    while (i != len) {
        pt = m[i]; //m is ascii code for character at [i] index in msg
        pt = pt - 96; //subtracted 96 to get the english alphabetical order used for calc. cipher
        k = 1;
        for (j = 0; j < key; j++) {
            k = k * pt; //m^e
            k = k % n; //m^e.modn, yeah!! we got the cipher for the letter here in alphabetical number
        }
        temp[i] = k; //for use in the decryption algo
        ct = k + 96; //back to ascii characters
        en[i] = ct; //inserting them into the encoded array
        i++;
    }
    en[i] = -1;
    char writestr[500];

    for (i = 0; en[i] != -1; i++) {
        writestr[i] = en[i]; //converting ascii to string values
    }
    f.write(writestr, strlen(writestr)); //storing into f file having encrypted text
    f.close();

    cout << "\nTHE ENCRYPTED MESSAGE IS\n";
    cout << "\n" << writestr << "\n"; //printing the encoded message
}

void decrypt() {
    long int pt, ct, key = d[4], k;
    i = 0;
    while (en[i] != -1) {
        ct = temp[i]; //using temp[] from encrypt() having cipher in apha number form
        k = 1;
        for (j = 0; j < key; j++) {
            k = k * ct; //c^d
            k = k % n; //c^d.modn here, we got the decrypted message in alphabetical number format
        }
        pt = k + 96; //getting back the ascii value
        m[i] = pt; //putting the ascii values into the m[] array
        i++;
    }
    m[i] = -1;
    cout << "\n\nTHE DECRYPTED MESSAGE IS\n";
    for (i = 0; m[i] != -1; i++)
        printf("%c", m[i]); //converting ascii to alphabets
}


