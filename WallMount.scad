$fn=128;

backDepth=7;
backHeight=60;

trayDepth=65;
trayHeight=10;

shelfWidth=90;
shelfRadius=3;

screwRadius=1.7;
nutRadius=3.2;
nutHeight=3;

pinRadius=1;

//Pin hole
module pinHole(x=0,y=0,z=0) {
    translate([x,y,z]) {
        rotate([-45,0,0]){
            cylinder(h=3*backDepth, 
                     r=pinRadius, 
                     center=true);
        }
    }
}

//Screw hole
module screwHole(x=0,y=0) {
    translate([x,y,-(trayHeight/2)-1]) {        
        cylinder(h=2+trayHeight, 
                 r=screwRadius, 
                 center=false);
        cylinder(h=1+nutHeight, 
                 r=nutRadius, 
                 center=false,
                 $fn=6);
    }
}

//Uno holes
module unoHoles(x=0,y=0) {
    translate([x,y,0]) {        
        screwHole(x=(-35+(0.55*25.4)), y=(5+(0.1*25.4)));
        screwHole(x=(-35+(2.55*25.4)), y=(5+(0.2*25.4)));
        screwHole(x=(-35+(0.55*25.4)), y=(5+(1.9*25.4)));
        screwHole(x=(-35+(2.55*25.4)), y=(5+(1.3*25.4)));
    }
}

//Shelf
module shelf(x=0,y=0) {
        union() {
  
//           //TGGAA
//           translate ([0,-(backDepth/2),backHeight-8]) {
//               rotate ([90,0,180]) {
//                   linear_extrude(height=backDepth, slices=2, center=true) {
//                       text("TGGAA",
//                            valign="center",
//                            halign="center",
//                            size=14,
//                            font="Tw Cen MT Condensed Extra Bold:style=Regular");
//                   }
//                }
//            }   
            
           //Projekttage
           translate ([0,2-(backDepth/2),backHeight-20]) {
               rotate ([90,0,180]) {
                   linear_extrude(height=backDepth+1, slices=2, center=true) {
                       text("Projekttage",
                            valign="center",
                            halign="center",
                            size=14,
                            font="Tw Cen MT Condensed Extra Bold:style=Regular");
                   }
                }
            }   

           //2016
           translate ([0,2-(backDepth/2),backHeight-35]) {
               rotate ([90,0,180]) {
                   linear_extrude(height=backDepth+1, slices=2, center=true) {
                       text("2016",
                            valign="center",
                            halign="center",
                            size=14,
                            font="Tw Cen MT Condensed Extra Bold:style=Regular");
                   }
                }
            }   

            
            
            
            //Tray
            translate([0,(trayDepth/2),0]) {
                difference() {
                    minkowski() {
                        cube(size=[shelfWidth-(2*shelfRadius),
                                   trayDepth-(2*shelfRadius),
                                   trayHeight-(2*shelfRadius)], center=true);
                        sphere(r=shelfRadius); 
                    }
                    unoHoles(y=shelfRadius-(trayDepth/2));
                 }    
           }
            //Back
            translate([0,-(backDepth/2),((backHeight/2)-(2*shelfRadius)-(trayHeight/2))]) { 
                difference() {
                    minkowski() {
                       cube(size=[shelfWidth-(2*shelfRadius),
                                  backDepth-(2*shelfRadius),
                                  backHeight-(2*shelfRadius)], center=true);
                        sphere(r=shelfRadius); 
                    }
                    union () {
                        pinHole (x=-(0.45*shelfWidth), z=(backHeight/2)-(backDepth));
                        pinHole (x=-(0.45*shelfWidth), z=2-(2*shelfRadius)-(trayHeight/2));
                        pinHole (x= (0.45*shelfWidth), z=(backHeight/2)-(backDepth));
                        pinHole (x= (0.45*shelfWidth), z=2-(2*shelfRadius)-(trayHeight/2));
                    }
                }    
            }
            //Connector
            minkowski() {
                difference() {
                    translate([0,0,0]) { 
                            cube(size=[shelfWidth-(2*shelfRadius),
                                                  (2*shelfRadius),
                                                   trayHeight+(2*shelfRadius)], center=true);
                    }
                    rotate([0,90,0]) {
                        translate([shelfRadius+(trayHeight/2),shelfRadius,0]) { 
                            cylinder(h=10+shelfWidth-(2*shelfRadius), 
                                     r=(2*shelfRadius), 
                                     center=true);    
                        }
                        translate([-shelfRadius-(trayHeight/2),shelfRadius,0]) { 
                            cylinder(h=10+shelfWidth-(2*shelfRadius), 
                                     r=(2*shelfRadius), 
                                     center=true);    
                        }
                    
                    }        
                }
                sphere(r=shelfRadius); 
              }
        }
 
}

rotate ([90,0,0]) {
    translate ([0, backDepth,0]) {
        shelf();
    }
}
//unoHoles(y=shelfRadius);