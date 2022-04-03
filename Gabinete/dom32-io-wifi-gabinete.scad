origen = [0,0,0];

placa_x = 80;
placa_y = 65;
wifi_xy = 25;
espesor_placa = 2;
espesor_paredes = 1.5;
espesor_base = 2;
espesor_superior = 2;
separacion_xyz = 0.25;
separador_z = 5;
borde_apoyo = 1.5;
bornera_z = 10;
relay_z = 20;
borneras_frente_x = 2.25;
borneras_frente_len = 26;
borneras_fondo_x = 2.25;
borneras_fondo_len = 42;
escalon_borneras_y = 11;
boton_x = 60;
boton_y = 6;
separacion_leds = 14.5;
ventilacion = 1;
ventilacion_frente_x = 5;
tornillo_bornera_frente_x = 7;
tornillo_bornera_fondo_x = 7;
tornillo_bornera_y = 7;
tornillo_bornera_diametro = 4;
$fn = 100;

caja_x = placa_x + (2*espesor_paredes) + (2*separacion_xyz);
caja_y = placa_y + (2*espesor_paredes) + (2*separacion_xyz);
caja_z = espesor_base + espesor_placa + bornera_z + separacion_xyz + separador_z + espesor_paredes;
interior_x = placa_x + (2*separacion_xyz);
interior_y = placa_y + (2*separacion_xyz);
interior_z = caja_z - espesor_base - separador_z;


// -- Base --
module Ventilacion()
{
    for(i = [0:3:20])
    {
    translate(origen + [(ventilacion_frente_x + (ventilacion * i)),(caja_y-(caja_z/2)),0])
        cube([ventilacion,caja_z/2,caja_z/2], center=false);
    }
}

module CaladoTapaMedia()
{
    translate(origen + [espesor_paredes/2, espesor_paredes/2, caja_z-espesor_paredes])
        cube([caja_x-espesor_paredes,caja_y-espesor_paredes,espesor_paredes] , center=false);
}

module CaladoAntenaWiFi()
{
    translate(origen + [espesor_paredes, caja_y - wifi_xy - espesor_paredes, separador_z])
        cube([wifi_xy,wifi_xy,espesor_placa], center=false);
}

module VentanaBorneraFrente()
{
    translate(origen+[espesor_paredes+borneras_frente_x,0,espesor_base+separador_z+espesor_placa])
        cube([borneras_frente_len,espesor_paredes,caja_z -espesor_base - separador_z - espesor_placa], center=false);
}

module VentanaBorneraFondo()
{
    translate(origen+[-espesor_paredes-borneras_frente_x,0,espesor_base+separador_z+espesor_placa]+[caja_x,caja_y,0])
    rotate([0,0,180])
        cube([borneras_fondo_len,espesor_paredes,caja_z -espesor_base - separador_z - espesor_placa], center=false);
}

module Cajon()
{
    difference()
    {
        // Cubo exterior
        translate(origen)
            cube([caja_x,caja_y,caja_z],center=false);
        // Calado del interfior
        translate(origen + [espesor_paredes,espesor_paredes,espesor_base+separador_z])
            cube([interior_x,interior_y,interior_z],center=false);

        translate(origen+[espesor_paredes+borde_apoyo,espesor_paredes+borde_apoyo,espesor_base])
            cube([placa_x-(2*borde_apoyo),placa_y-(2*borde_apoyo),caja_z],center=false);
    }
}

module Base()
{
    difference()
    {
        Cajon();
        CaladoAntenaWiFi();
        CaladoTapaMedia();
        VentanaBorneraFrente();
        VentanaBorneraFondo();
        Ventilacion();
    }
}

// -- Tapa superior --

module TapaSuperior()
{
        translate(origen + [espesor_paredes/2, espesor_paredes/2, caja_z-espesor_paredes] + [0, escalon_borneras_y, 0] + [espesor_paredes/2,espesor_paredes/2,relay_z-caja_z+separador_z+espesor_placa+espesor_base])
            cube([caja_x-(2*espesor_paredes),caja_y-(2*escalon_borneras_y)-(2*espesor_paredes),espesor_superior] , center=false);
}

// -- Tapa media --

module CajonRelays()
{
    translate(origen + [espesor_paredes/2, espesor_paredes/2, caja_z-espesor_paredes] + [0, escalon_borneras_y, 0])
        cube([caja_x-espesor_paredes,caja_y-espesor_paredes-(2*escalon_borneras_y),relay_z-caja_z+separador_z+espesor_placa+espesor_base+espesor_superior] , center=false);
}

module TapaBase()
{
    translate(origen + [espesor_paredes/2, espesor_paredes/2, caja_z-espesor_paredes])
        cube([caja_x-espesor_paredes,caja_y-espesor_paredes,espesor_paredes] , center=false);
   
    CajonRelays();
}

module HuecoTapaMedia()
{
        translate(origen + [espesor_paredes/2, espesor_paredes/2, caja_z-espesor_paredes] + [0, escalon_borneras_y, 0] + [espesor_paredes,espesor_paredes,0])
            cube([caja_x-espesor_paredes-(2*espesor_paredes),caja_y-espesor_paredes-(2*escalon_borneras_y)-(2*espesor_paredes),relay_z-caja_z+separador_z+espesor_placa+espesor_base+espesor_superior] , center=false);
}

module TornilloBorneraFrente()
{
    for(i=[0:5:20])
    {
        translate(origen + [tornillo_bornera_frente_x+i, tornillo_bornera_y, caja_z-espesor_paredes])
            cylinder(h=espesor_superior, d=tornillo_bornera_diametro,center=false);
    }
}

module TornilloBorneraFondo()
{
    for(i=[0:5:35])
    {
        translate(origen + [caja_x,caja_y,caja_z-espesor_paredes] - [tornillo_bornera_frente_x+i, tornillo_bornera_y, 0])
            cylinder(h=espesor_superior, d=tornillo_bornera_diametro,center=false);
    }
}

module TapaMedia()
{
    difference()
    {
        TapaBase();
        HuecoTapaMedia();
        TapaSuperior();
        TornilloBorneraFrente();
        TornilloBorneraFondo();
    }
}


// -- Main

Base();
TapaMedia();
TapaSuperior();
