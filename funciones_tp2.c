#include "funciones_tp2.h"

//-----------------------------------------------------------------

bool existe_paciente(hash_t* pacientes, char* paciente) {
    return hash_pertenece(pacientes, paciente);
}

bool existe_doctor(hash_t* doctores, char* doctor) {
    return hash_pertenece(doctores, doctor);
}

bool existe_especialidad(hash_t* cant_pacientes_esp, char* especialidad) {
    return hash_pertenece(cant_pacientes_esp, especialidad);
}

bool existe_urgencia(char* urgencia) {
    return ((strcmp(urgencia, "URGENTE") == 0) || (strcmp(urgencia, "REGULAR") == 0));
}

void pedir_turno(hospital_t* hospital, char** turno) { 
    
    if (!existe_paciente(hospital_pacientes(hospital), turno[0])) {
        printf(ENOENT_PACIENTE, turno[0]);
    } if (!existe_especialidad(hospital_cantidad(hospital), turno[1])) {
        printf(ENOENT_ESPECIALIDAD, turno[1]);
    } if (!existe_urgencia(turno[2])) {
        printf(ENOENT_URGENCIA, turno[2]);
    } else {
        if(strcmp(turno[2], "URGENTE") == 0){
            cola_t* urgentes = obtener_cola_prioridad(hash_obtener(hospital_turnos(hospital), turno[1]));
            cola_encolar(urgentes,hash_obtener(hospital_pacientes(hospital), turno[0]));
        }else{
            abb_t* regulares = obtener_abb_prioridad(hash_obtener(hospital_turnos(hospital), turno[1]));
            paciente_t* paciente = hash_obtener(hospital_pacientes(hospital), turno[0]);
            abb_guardar(regulares, obtener_inscripcion_paciente(paciente), paciente);
        }
        size_t cantidad = *((size_t*)hash_obtener(hospital_cantidad(hospital), turno[1]));
        cantidad ++;
        hash_guardar(hospital_cantidad(hospital), turno[1], (void*)cantidad);
        cantidad = *((size_t*)hash_obtener(hospital_cantidad(hospital), turno[1]));
        printf(PACIENTE_ENCOLADO, turno[0]);
        printf(CANT_PACIENTES_ENCOLADOS,cantidad, turno[1]);
    }
}

char* abb_eliminar_paciente(abb_t* abb) {
    paciente_t* paciente;
    abb_iter_t* iter = abb_iter_in_crear(abb);
    char* resultado;
   
    while (!abb_iter_in_al_final(iter)) abb_iter_in_avanzar(iter);
    paciente = (paciente_t*)abb_borrar(abb ,abb_iter_in_ver_actual(iter));
    resultado = obtener_nombre_paciente(paciente);

    return resultado;
}


void atender_sgte_paciente(hospital_t* hospital, char** linea) {
    char* especialidad, *paciente;
    size_t cantidad;
    cola_t* urgentes;
    abb_t* regulares;
    doctor_t* doctor;
    
    if (!existe_doctor(hospital_doctores(hospital), linea[0])) {
        printf(ENOENT_DOCTOR, linea[0]);
    } else {
        doctor = hash_obtener(hospital_doctores(hospital), linea[0]);
        especialidad = obtener_especialidad_doctor(doctor);
        cantidad = *(size_t*)hash_obtener(hospital_cantidad(hospital), especialidad);
        urgentes = obtener_cola_prioridad(hash_obtener(hospital_turnos(hospital),linea[0]));
        regulares = obtener_abb_prioridad(hash_obtener(hospital_turnos(hospital),linea[0]));
        if (!cola_esta_vacia(urgentes)) {
            paciente = desencolar_nombre(urgentes);
            printf(PACIENTE_ATENDIDO, paciente);
        } else if (abb_cantidad(regulares) > 0)  {
            paciente = abb_eliminar_paciente(regulares);
            printf(PACIENTE_ATENDIDO, paciente);
        } else {
            printf(SIN_PACIENTES);
        }
        
        if (cantidad > 0) {
            cantidad --;
            printf(CANT_PACIENTES_ENCOLADOS, cantidad, especialidad);
            incrementar_atendidos(hospital,doctor);
            incrementar_atendidos_abb(hospital, doctor);
            hash_guardar(hospital_cantidad(hospital), especialidad, (void*)cantidad);
        }   
    }  
}

void informe_doctores(hospital_t* hospital, char* inicio, char* fin) {
    hash_t* hash_doc = hospital_doctores(hospital);
    if(!hash_pertenece(hash_doc, inicio)){
        printf(ENOENT_DOCTOR, inicio);
        return;
    }
    if(!hash_pertenece(hash_doc, fin)){
        printf(ENOENT_DOCTOR, fin);
        return;
    }
    cola_t* cola = cola_crear();
    abb_t* abb_doctores = hospital_abb_doctore(hospital);
    abb_iter_t* iter = abb_iter_in_crear(abb_doctores);
    size_t contador_1 = 0, cant_atendidos = 0;
    char* nombre, *especialidad;


    while (!abb_iter_in_al_final(iter)) {
        if (strcmp(abb_iter_in_ver_actual(iter), fin) <= 0 &&
            strcmp(abb_iter_in_ver_actual(iter), inicio) >= 0) {
            contador_1 ++;
            cola_encolar(cola, abb_obtener(abb_doctores, abb_iter_in_ver_actual(iter)));
            }
            abb_iter_in_avanzar(iter);
        }
    
    printf(DOCTORES_SISTEMA, contador_1);
    for (size_t i = 1; !cola_esta_vacia(cola); i++) {
        doctor_t* doctor = (doctor_t*)cola_desencolar(cola);
        nombre = obtener_nombre_doctor(doctor);
        especialidad = obtener_especialidad_doctor(doctor);
        cant_atendidos = obtener_cant_atendidos_doctor(doctor);
        printf(INFORME_DOCTOR ,i, nombre, especialidad, cant_atendidos);
    }
}

