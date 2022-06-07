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
bornera_z = 12;
relay_z = 25;
borneras_frente_x = 2.25;
borneras_frente_len = 26;
borneras_fondo_x = 2.25;
borneras_fondo_len = 42;
escalon_borneras_y = 10;
boton_x = 62;
boton_y = 8;
boton = [5,5,0];
led_x = 45;
led_y = 8;
led = [2,5,0];
separacion_leds = 14.5;
ventilacion = 1;
ventilacion_frente_x = 5;
tornillo_bornera_frente_x = 5;
tornillo_bornera_fondo_x = 5;
tornillo_bornera_y = 6;
tornillo_bornera_diametro = 4;
tornillo_fijacion_diametro = 3.5;
oreja_fijacion = 10;
tol = 0.25;
$fn = 100;

caja_x = placa_x + (2*espesor_paredes) + (2*separacion_xyz);
caja_y = placa_y + (2*espesor_paredes) + (2*separacion_xyz);
caja_z = espesor_base + espesor_placa + bornera_z + separacion_xyz + separador_z + espesor_paredes;
interior_x = placa_x + (2*separacion_xyz);
interior_y = placa_y + (2*separacion_xyz);
interior_z = caja_z - espesor_base - separador_z;

// -- Logo --
module Logo()
{
    translate(origen + [espesor_paredes/2, espesor_paredes/2, caja_z-espesor_paredes] + [0, escalon_borneras_y, 0] + [espesor_paredes/2,espesor_paredes/2,relay_z-caja_z+separador_z+espesor_placa+espesor_base+(espesor_superior/4*3)] + [10,30,0])
        linear_extrude(espesor_superior/4)
            text("Dom32 IO WiFi", size = 6, font = "Liberation Sans:style=Bold Italic");
}

// -- Base --
module Oreja(x, y, dir)
{
    if(dir == 1)
    {
        translate([x-oreja_fijacion, y, 0])
            cube([oreja_fijacion, oreja_fijacion, 3], center = false);
        translate([x-oreja_fijacion, y+(oreja_fijacion/2), 0])
            cylinder(h=3, d=oreja_fijacion, center=false);
    }
    else
    {
        translate([x, y, 0])
            cube([oreja_fijacion, oreja_fijacion, 3], center = false);
        translate([x+oreja_fijacion, y+(oreja_fijacion/2), 0])
            cylinder(h=3, d=oreja_fijacion, center=false);
    }
}

module OrejaTornillo(x, y, dir)
{
    if(dir == 1)
    {
        difference()
        {
            Oreja(x, y, dir);
            translate([x-oreja_fijacion, y+(oreja_fijacion/2), 0])
                cylinder(h=3, d=tornillo_fijacion_diametro, center=false);
        }
    }
    else
    {
        difference()
        {
            Oreja(x, y, dir);
            translate([x+oreja_fijacion, y+(oreja_fijacion/2), 0])
                cylinder(h=3, d=tornillo_fijacion_diametro, center=false);
        }
    }
}

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
    OrejaTornillo(origen.x, origen.y + 5, 1);
    OrejaTornillo(origen.x, caja_y-15, 1);
    OrejaTornillo(caja_x, origen.y + 5, 2);
    OrejaTornillo(caja_x, caja_y-15, 2);
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
    difference()
    {
        translate(origen + [espesor_paredes/2 + tol, espesor_paredes/2 + tol, caja_z-espesor_paredes] + [0, escalon_borneras_y, 0] + [espesor_paredes/2,espesor_paredes/2,relay_z-caja_z+separador_z+espesor_placa+espesor_base])
            cube([caja_x-(2*espesor_paredes) - 2*tol,caja_y-(2*escalon_borneras_y)-(2*espesor_paredes) - 2*tol,espesor_superior] , center=false);
        
    }
}

// -- Tapa media --

module CajonRelays()
{
    translate(origen + [espesor_paredes/2 + tol/2, espesor_paredes/2 + tol/2, caja_z-espesor_paredes] + [0, escalon_borneras_y, 0])
        cube([caja_x-espesor_paredes-tol,caja_y-espesor_paredes-(2*escalon_borneras_y)-tol,relay_z-caja_z+separador_z+espesor_placa+espesor_base+espesor_superior] , center=false);
}

module TapaBase()
{
    translate(origen + [espesor_paredes/2 + tol/2, espesor_paredes/2 + tol/2, caja_z-espesor_paredes])
        cube([caja_x-espesor_paredes-tol,caja_y-espesor_paredes - tol,espesor_paredes] , center=false);
   
    CajonRelays();
}

module HuecoTapaMedia()
{
        translate(origen + [espesor_paredes/2 + tol/2, espesor_paredes/2 + tol/2, caja_z-espesor_paredes] + [0, escalon_borneras_y, 0] + [espesor_paredes,espesor_paredes,0])
            cube([caja_x-espesor_paredes-(2*espesor_paredes)-tol,caja_y-espesor_paredes-(2*escalon_borneras_y)-(2*espesor_paredes)-tol,relay_z-caja_z+separador_z+espesor_placa+espesor_base+espesor_superior] , center=false);
}

module TornilloBorneraFrente()
{
    for(i=[0:5:20])
    {
        translate(origen + [tornillo_bornera_frente_x+i+espesor_paredes, tornillo_bornera_y+espesor_paredes, caja_z-espesor_paredes])
            cylinder(h=espesor_superior, d=tornillo_bornera_diametro,center=false);
    }
}

module TornilloBorneraFondo()
{
    for(i=[0:5:35])
    {
        translate(origen + [caja_x,caja_y,caja_z-espesor_superior] - [tornillo_bornera_frente_x+i+espesor_paredes, tornillo_bornera_y+espesor_paredes, 0])
            cylinder(h=espesor_superior, d=tornillo_bornera_diametro,center=false);
    }
}

module OrificioPulsador()
{
    translate(origen + [boton_x-(boton.x/2), boton_y-(boton.y/2), caja_z-espesor_paredes])
        cube([boton.x,boton.y,espesor_superior], center=false);
}

module OrificioLed()
{
    translate(origen + [led_x-(led.x/2), led_y-(led.y/2), caja_z-espesor_paredes])
        cube([led.x,led.y,espesor_superior], center=false);
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
        OrificioPulsador();
        OrificioLed();
    }
}

module TapaConLogo()
{
    difference()
    {
        TapaSuperior();
        Logo();
    }
}

module Pulsador()
{
    translate(origen)
        cube([4,4,8], center = false);
    translate(origen - [8,0,0])
        cube([20,9,4.5], center = false);
}

module OjoDeBuey()
{
    translate(origen)
        cube([5,4,9], center = false);
    translate(origen + [1.6,0,9])
        cube([1.8,4,2], center = false);
    
}

// -- Main

Base();
//TapaMedia();
//TapaConLogo();
//Pulsador();
//OjoDeBuey();
//Logo();