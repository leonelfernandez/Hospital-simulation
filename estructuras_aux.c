#include "estructuras_aux.h"

struct hospital{
	hash_t* pacientes;
	hash_t* doctores;
    hash_t* cant_pacientes_esp; //Cant de pacientes por especialidad
    hash_t* turnos_pedidos; //Hash de colas/abbs por especialidad
    abb_t* abb_doctore;
};

struct doctores {
    char* nombre;
    char* especialidad;
    size_t cant_atendidos;
};

struct pacientes {
    char* nombre;
    char* inscripcion;
};

struct prioridades {
    cola_t* urgentes;
    abb_t* regulares;
};

// --------------------- CREADORES ------------------------------------
doctor_t* crear_doctor(char** datos) {
    doctor_t* doctor = malloc(sizeof(doctor_t));
    if (!doctor) return NULL;
    doctor->nombre = datos[0];
    doctor->especialidad = datos[1];
    doctor->cant_atendidos = 0;
    return doctor;
}

paciente_t* crear_paciente(char** datos) {
    paciente_t* paciente = malloc(sizeof(paciente_t));
    if (!paciente) return NULL;
    paciente->nombre = datos[0];
    paciente->inscripcion = datos[1];
    return paciente;
}

prioridades_t* crear_prioridades() {
    prioridades_t* prioridades = malloc(sizeof(prioridades_t));
    if (!prioridades) return NULL;
    prioridades->regulares = abb_crear(strcmp, NULL);
    if (!prioridades->regulares) return NULL;
    prioridades->urgentes = cola_crear();
    if (!prioridades->urgentes) return NULL;
    return prioridades;
}

hospital_t* crear_hospital(hash_t* pacientes, hash_t* doctores, hash_t* cant_pacientes_esp, hash_t* turnos_pedidos, abb_t* abb_doctore) {
	hospital_t* hospital = malloc(sizeof(hospital_t));
	if (!hospital) return NULL;
	hospital->pacientes = pacientes;
	hospital->doctores = doctores;
	hospital->cant_pacientes_esp = cant_pacientes_esp;
    hospital->turnos_pedidos = turnos_pedidos;
    hospital->abb_doctore = abb_doctore;
    return hospital;
}

hash_t* crear_hash_doctores(lista_t* doctores) {
    if (!doctores) return NULL;
    hash_t* hash = hash_crear(NULL);
    if (!hash) return NULL;
    lista_iter_t* iter = lista_iter_crear(doctores);
    while (!lista_iter_al_final(iter)) {
        hash_guardar(hash, ((doctor_t*)lista_iter_ver_actual(iter))->nombre, (doctor_t*)lista_iter_ver_actual(iter));
        lista_iter_avanzar(iter);
    }
    return hash;
}

char* obtener_especialidad_doctor(doctor_t* doctor){
    return doctor->especialidad;
}

char* obtener_nombre_doctor(doctor_t* doctor){
    return doctor->nombre;
}

size_t obtener_cant_atendidos_doctor(doctor_t* doctor){
    return doctor->cant_atendidos;
}

int es_digito(char* string) {
    
    for (int i = 0; string[i]; i++) {
        if (isdigit(string[i]) || string[i] == '-') {
            continue;
        } else {
            return 0;
        }
    }
    return 1;
} 

hash_t* crear_hash_pacientes(lista_t* pacientes) {
    if (!pacientes) return NULL;
    hash_t* hash = hash_crear(NULL);
    if (!hash) return NULL;
    lista_iter_t* iter = lista_iter_crear(pacientes);
    while (!lista_iter_al_final(iter)) {
        if(!es_digito(((paciente_t*)lista_iter_ver_actual(iter))->inscripcion)){
            printf(ENOENT_ANIO,((paciente_t*)lista_iter_ver_actual(iter))->inscripcion);
            return NULL;
        }
        hash_guardar(hash, ((paciente_t*)lista_iter_ver_actual(iter))->nombre, (paciente_t*)lista_iter_ver_actual(iter));
        
        lista_iter_avanzar(iter);
    }
    return hash;
}

hash_t* crear_hash_cant_x_especialidad(lista_t* doctores) {
    if (!doctores) return NULL;
    hash_t* hash = hash_crear(NULL);
    if (!hash) return NULL;
    lista_iter_t* iter = lista_iter_crear(doctores);
    while (!lista_iter_al_final(iter)) {
        hash_guardar(hash, ((doctor_t*)lista_iter_ver_actual(iter))->especialidad, 0);
        lista_iter_avanzar(iter);
    }
    return hash;
}

hash_t* crear_hash_a_atender(lista_t* doctores) {
    if (!doctores) return NULL;
    hash_t* hash = hash_crear(NULL);
    if (!hash) return NULL;
    lista_iter_t* iter = lista_iter_crear(doctores);
    while (!lista_iter_al_final(iter)) {
        hash_guardar(hash, ((doctor_t*)lista_iter_ver_actual(iter))->especialidad, crear_prioridades());
        lista_iter_avanzar(iter);
    }
    return hash;
}

abb_t* crear_abb_doctores(lista_t* doctores) {
    if (!doctores) return NULL;
    abb_t* abb = abb_crear(strcmp,NULL);
    if (!abb) return NULL;
    lista_iter_t* iter = lista_iter_crear(doctores);
    while (!lista_iter_al_final(iter)) {
        abb_guardar(abb, ((doctor_t*)lista_iter_ver_actual(iter))->nombre, (doctor_t*)lista_iter_ver_actual(iter));
        lista_iter_avanzar(iter);
    }
    return abb;
}

// -----------------------------------------------------------------------------

hash_t* hospital_cantidad(hospital_t* hospital){
    if(!hospital) return NULL;
    return hospital->cant_pacientes_esp;
}

abb_t* hospital_abb_doctore(hospital_t* hospital){
    if(!hospital) return NULL;
    return hospital->abb_doctore;
}

hash_t* hospital_turnos(hospital_t* hospital){
    if(!hospital) return NULL;
    return hospital->turnos_pedidos;
}

hash_t* hospital_doctores(hospital_t* hospital){
    if(!hospital) return NULL;
    return hospital->doctores;
}

hash_t* hospital_pacientes(hospital_t* hospital){
    if(!hospital) return NULL;
    return hospital->pacientes;
}

char* obtener_inscripcion_paciente(paciente_t* paciente){
    return paciente->inscripcion;
}

char* obtener_nombre_paciente(paciente_t* paciente){
    return paciente->nombre;
}

cola_t* obtener_cola_prioridad(prioridades_t* prioridad){
    return prioridad->urgentes;
}

abb_t* obtener_abb_prioridad(prioridades_t* prioridad){
    return prioridad->regulares;
}

char* desencolar_nombre(cola_t* cola) {
    paciente_t* paciente = (paciente_t*)cola_desencolar(cola);
    return paciente->nombre;
}

void incrementar_atendidos(hospital_t* hospital,doctor_t* doctor){
    doctor->cant_atendidos ++;
    hash_guardar(hospital->doctores, doctor->nombre, doctor);
}

void incrementar_atendidos_abb(hospital_t* hospital, doctor_t* doctor){
    doctor->cant_atendidos ++;
    abb_guardar(hospital->abb_doctore, doctor->nombre, doctor);
}

void hospital_destruir(hospital_t* hospital){
    hash_destruir(hospital->cant_pacientes_esp);
    hash_destruir(hospital->doctores);
    hash_destruir(hospital->pacientes);
    hash_destruir(hospital->turnos_pedidos);
    abb_destruir(hospital->abb_doctore);
}

void hospital_destruir_listas(lista_t* lista_doctores, lista_t* lista_pacientes){
    lista_destruir(lista_doctores,NULL);
    lista_destruir(lista_pacientes,NULL);
}