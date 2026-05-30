#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PATIENTS 100
#define MAX_DOCTORS 20
#define MAX_APPOINTMENTS 200
#define FILE_PATIENTS "patients.dat"
#define FILE_DOCTORS "doctors.dat"
#define FILE_APPOINTMENTS "appointments.txt"

// ─── STRUCTS ───────────────────────────────────────────────
typedef struct {
    int id;
    char name[50];
    int age;
    char gender[10];
    char phone[15];
    char diagnosis[100];
    char doctor_assigned[50];
} Patient;

typedef struct {
    int id;
    char name[50];
    char specialization[50];
    char phone[15];
} Doctor;

typedef struct {
    int appointment_id;
    int patient_id;
    int doctor_id;
    char date[20];
    char time[10];
    char status[20];
} Appointment;

// ─── GLOBALS ───────────────────────────────────────────────
Patient patients[MAX_PATIENTS];
Doctor  doctors[MAX_DOCTORS];
Appointment appointments[MAX_APPOINTMENTS];

int patient_count     = 0;
int doctor_count      = 0;
int appointment_count = 0;

// ─── HELPER ────────────────────────────────────────────────
void to_lower(char *dest, const char *src) {
    int i;
    for (i = 0; src[i]; i++)
        dest[i] = tolower((unsigned char)src[i]);
    dest[i] = '\0';
}

// ─── FILE I/O ──────────────────────────────────────────────
void save_patients() {
    FILE *fp = fopen(FILE_PATIENTS, "wb");
    if (!fp) { printf("Error saving patients.\n"); return; }
    fwrite(&patient_count, sizeof(int), 1, fp);
    fwrite(patients, sizeof(Patient), patient_count, fp);
    fclose(fp);
    printf("Patients saved.\n");
}

void load_patients() {
    FILE *fp = fopen(FILE_PATIENTS, "rb");
    if (!fp) return;
    fread(&patient_count, sizeof(int), 1, fp);
    fread(patients, sizeof(Patient), patient_count, fp);
    fclose(fp);
}

void save_doctors() {
    FILE *fp = fopen(FILE_DOCTORS, "wb");
    if (!fp) { printf("Error saving doctors.\n"); return; }
    fwrite(&doctor_count, sizeof(int), 1, fp);
    fwrite(doctors, sizeof(Doctor), doctor_count, fp);
    fclose(fp);
    printf("Doctors saved.\n");
}

void load_doctors() {
    FILE *fp = fopen(FILE_DOCTORS, "rb");
    if (!fp) return;
    fread(&doctor_count, sizeof(int), 1, fp);
    fread(doctors, sizeof(Doctor), doctor_count, fp);
    fclose(fp);
}

void save_appointments_text() {
    FILE *fp = fopen(FILE_APPOINTMENTS, "w");
    if (!fp) { printf("Error saving appointments.\n"); return; }
    fprintf(fp, "%-5s %-10s %-10s %-12s %-8s %-15s\n",
            "AID", "PatientID", "DoctorID", "Date", "Time", "Status");
    fprintf(fp, "--------------------------------------------------------------\n");
    for (int i = 0; i < appointment_count; i++) {
        fprintf(fp, "%-5d %-10d %-10d %-12s %-8s %-15s\n",
                appointments[i].appointment_id,
                appointments[i].patient_id,
                appointments[i].doctor_id,
                appointments[i].date,
                appointments[i].time,
                appointments[i].status);
    }
    fclose(fp);
    printf("Appointments saved to %s.\n", FILE_APPOINTMENTS);
}

// ─── PATIENT FUNCTIONS ─────────────────────────────────────
void add_patient() {
    if (patient_count >= MAX_PATIENTS) {
        printf("Patient limit reached.\n"); return;
    }
    Patient p;
    p.id = patient_count + 1;
    printf("\n--- Add Patient ---\n");
    printf("Name        : "); scanf(" %[^\n]", p.name);
    printf("Age         : "); scanf("%d", &p.age);
    printf("Gender      : "); scanf(" %s", p.gender);
    printf("Phone       : "); scanf(" %s", p.phone);
    printf("Diagnosis   : "); scanf(" %[^\n]", p.diagnosis);
    printf("Doctor Name : "); scanf(" %[^\n]", p.doctor_assigned);

    patients[patient_count++] = p;
    printf("Patient added with ID: %d\n", p.id);
}

void search_patient() {
    char name[50];
    printf("Enter patient name to search: ");
    scanf(" %[^\n]", name);

    char lname[50], lpname[50];
    to_lower(lname, name);

    int found = 0;
    for (int i = 0; i < patient_count; i++) {
        to_lower(lpname, patients[i].name);
        if (strstr(lpname, lname)) {
            printf("\n--- Patient Found ---\n");
            printf("ID          : %d\n", patients[i].id);
            printf("Name        : %s\n", patients[i].name);
            printf("Age         : %d\n", patients[i].age);
            printf("Gender      : %s\n", patients[i].gender);
            printf("Phone       : %s\n", patients[i].phone);
            printf("Diagnosis   : %s\n", patients[i].diagnosis);
            printf("Doctor      : %s\n", patients[i].doctor_assigned);
            found = 1;
        }
    }
    if (!found) printf("No patient found with name '%s'.\n", name);
}

void display_all_patients() {
    if (patient_count == 0) { printf("No patients on record.\n"); return; }
    printf("\n%-4s %-20s %-4s %-8s %-15s %-25s %-20s\n",
           "ID","Name","Age","Gender","Phone","Diagnosis","Doctor");
    printf("----------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < patient_count; i++) {
        printf("%-4d %-20s %-4d %-8s %-15s %-25s %-20s\n",
               patients[i].id, patients[i].name, patients[i].age,
               patients[i].gender, patients[i].phone,
               patients[i].diagnosis, patients[i].doctor_assigned);
    }
}

// ─── DOCTOR FUNCTIONS ──────────────────────────────────────
void add_doctor() {
    if (doctor_count >= MAX_DOCTORS) {
        printf("Doctor limit reached.\n"); return;
    }
    Doctor d;
    d.id = doctor_count + 1;
    printf("\n--- Add Doctor ---\n");
    printf("Name           : "); scanf(" %[^\n]", d.name);
    printf("Specialization : "); scanf(" %[^\n]", d.specialization);
    printf("Phone          : "); scanf(" %s", d.phone);

    doctors[doctor_count++] = d;
    printf("Doctor added with ID: %d\n", d.id);
}

void display_all_doctors() {
    if (doctor_count == 0) { printf("No doctors on record.\n"); return; }
    printf("\n%-4s %-20s %-25s %-15s\n", "ID","Name","Specialization","Phone");
    printf("------------------------------------------------------------------\n");
    for (int i = 0; i < doctor_count; i++) {
        printf("%-4d %-20s %-25s %-15s\n",
               doctors[i].id, doctors[i].name,
               doctors[i].specialization, doctors[i].phone);
    }
}

// ─── APPOINTMENT FUNCTIONS ─────────────────────────────────
void schedule_appointment() {
    if (appointment_count >= MAX_APPOINTMENTS) {
        printf("Appointment limit reached.\n"); return;
    }
    Appointment a;
    a.appointment_id = appointment_count + 1;
    strcpy(a.status, "Scheduled");

    printf("\n--- Schedule Appointment ---\n");
    printf("Patient ID        : "); scanf("%d", &a.patient_id);
    printf("Doctor ID         : "); scanf("%d", &a.doctor_id);
    printf("Date (DD/MM/YYYY) : "); scanf(" %s", a.date);
    printf("Time (HH:MM)      : "); scanf(" %s", a.time);

    appointments[appointment_count++] = a;
    printf("Appointment scheduled. ID: %d\n", a.appointment_id);
}

void view_appointments() {
    if (appointment_count == 0) { printf("No appointments.\n"); return; }
    printf("\n%-5s %-10s %-10s %-12s %-8s %-15s\n",
           "AID","PatientID","DoctorID","Date","Time","Status");
    printf("--------------------------------------------------------------\n");
    for (int i = 0; i < appointment_count; i++) {
        printf("%-5d %-10d %-10d %-12s %-8s %-15s\n",
               appointments[i].appointment_id,
               appointments[i].patient_id,
               appointments[i].doctor_id,
               appointments[i].date,
               appointments[i].time,
               appointments[i].status);
    }
}

void update_appointment_status() {
    int id;
    printf("Enter Appointment ID to update: ");
    scanf("%d", &id);
    for (int i = 0; i < appointment_count; i++) {
        if (appointments[i].appointment_id == id) {
            printf("New status (Scheduled/Completed/Cancelled): ");
            scanf(" %s", appointments[i].status);
            printf("Status updated.\n");
            return;
        }
    }
    printf("Appointment ID %d not found.\n", id);
}

// ─── MENUS ─────────────────────────────────────────────────
void patient_menu() {
    int ch;
    do {
        printf("\n=== Patient Menu ===\n");
        printf("1. Add Patient\n");
        printf("2. Search Patient\n");
        printf("3. Display All Patients\n");
        printf("4. Save to File\n");
        printf("0. Back\n");
        printf("Choice: "); scanf("%d", &ch);
        switch (ch) {
            case 1: add_patient();          break;
            case 2: search_patient();       break;
            case 3: display_all_patients(); break;
            case 4: save_patients();        break;
            default: if(ch != 0) printf("Invalid choice.\n");
        }
    } while (ch != 0);
}

void doctor_menu() {
    int ch;
    do {
        printf("\n=== Doctor Menu ===\n");
        printf("1. Add Doctor\n");
        printf("2. Display All Doctors\n");
        printf("3. Save to File\n");
        printf("0. Back\n");
        printf("Choice: "); scanf("%d", &ch);
        switch (ch) {
            case 1: add_doctor();          break;
            case 2: display_all_doctors(); break;
            case 3: save_doctors();        break;
            default: if(ch != 0) printf("Invalid choice.\n");
        }
    } while (ch != 0);
}

void appointment_menu() {
    int ch;
    do {
        printf("\n=== Appointment Menu ===\n");
        printf("1. Schedule Appointment\n");
        printf("2. View All Appointments\n");
        printf("3. Update Appointment Status\n");
        printf("4. Save to Text File\n");
        printf("0. Back\n");
        printf("Choice: "); scanf("%d", &ch);
        switch (ch) {
            case 1: schedule_appointment();      break;
            case 2: view_appointments();         break;
            case 3: update_appointment_status(); break;
            case 4: save_appointments_text();    break;
            default: if(ch != 0) printf("Invalid choice.\n");
        }
    } while (ch != 0);
}

// ─── MAIN ──────────────────────────────────────────────────
int main() {
    load_patients();
    load_doctors();

    int choice;
    printf("==============================\n");
    printf("   Hospital Record System\n");
    printf("==============================\n");

    do {
        printf("\n=== Main Menu ===\n");
        printf("1. Patient Management\n");
        printf("2. Doctor Management\n");
        printf("3. Appointment Management\n");
        printf("0. Exit\n");
        printf("Choice: "); scanf("%d", &choice);
        switch (choice) {
            case 1: patient_menu();     break;
            case 2: doctor_menu();      break;
            case 3: appointment_menu(); break;
            case 0: printf("Goodbye!\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 0);

    return 0;
}