$fn=128;

backDepth=8;
backHeight=30;

trayDepth=65;
trayHeight=10;

shelfWidth=90;
shelfRadius=3;

screwRadius=1.7;
nutRadius=3.2;
nutHeight=3;


//Screw hole
module screwHole(x=0,y=0) {
    translate([x,y,-1]) {        
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
        //Tray
        translate([0,(trayDepth/2),0]) { 
            cube(size=[shelfWidth-(2*shelfRadius),
                       trayDepth-(2*shelfRadius),
                       trayHeight-(2*shelfRadius)], center=true);
        }
        //Back
        translate([0,-(backDepth/2),backHeight/2]) { 
            cube(size=[shelfWidth-(2*shelfRadius),
                       backDepth-(2*shelfRadius),
                       backHeight-(2*shelfRadius)], center=true);
        }
        //Connector
        //difference() {
        difference() {
            translate([0,0,0]) { 
                cube(size=[shelfWidth-(2*shelfRadius),
                                      (2*shelfRadius),
                                       trayHeight+(2*shelfRadius)], center=true);
            }
            rotate([0,90,0]) {
                translate([shelfRadius+(trayHeight/2),shelfRadius,0]) { 
                    cylinder(h=2+shelfWidth-(2*shelfRadius),, 
                             r=(2*shelfRadius), 
                             center=true);    
                }
                translate([-shelfRadius-(trayHeight/2),shelfRadius,0]) { 
                    cylinder(h=2+shelfWidth-(2*shelfRadius),, 
                             r=(2*shelfRadius), 
                             center=true);    
                }
            
            }        
        }


    }
        

//    difference() {
//            minkowski() {
//                sphere(r=trayRadius);
//            }
//        }
//        unoHoles();
//    }
}





//        //Dimensions=240
//        translate([0,0,height/2])
//            cube(size=[width-(2*radius),
//                       length-(2*radius),
//                       height-(2*radius)], center=true);
//        //Rounded corners
//        sphere(r=radius);
//    }

    
 

shelf();
unoHoles();

////Screw columns
//module screwColumns() {
//    for (rotation=[0,90,180,270]) {
//        rotate([0,0,rotation])
//            translate([18,18,18])
//                cylinder(h=height-pcbThickness, 
//                         r=screwRadius+screwClearance, 
//                         center=false);    
//    }




//width=52;
//height=25;
//radius=6;
//wallThickness=2;
//batHeight=17;
//batWidth=26.2;
//batGap=2;
//batLength=48.5;
//compHeight=2;
//pcbWidth=50.4;
//pcbRadius=5.2;
//pcbThickness=1;
//screwRadius=1.7;
//screwClearance=2;
//screwLength=10;
//nutRadius=3.1;
//overhangIncline=0.5;
//engraveDepth=0.5;
//
////Rounded Cuboid
//module roundedCuboid (width=50,
//                      length=50,
//                      height=20,
//                      radius=5) {
//    minkowski() {
//        //Dimensions=240
//        translate([0,0,height/2])
//            cube(size=[width-(2*radius),
//                       length-(2*radius),
//                       height-(2*radius)], center=true);
//        //Rounded corners
//        sphere(r=radius);
//    }
//}
//
////Outer body
//module outerBody() {
//    roundedCuboid(width=width,
//                  length=width,
//                  height=height+radius+1,
//                  radius=radius);
//}
//
////Height limit
//module heightLimit() {
//        translate([0,0,2*height])
//            cube(size=[width+2,
//                       width+2,
//                       2*height], center=true);
//}
//
////Battery space
//module batSpace() {
//    intersection() {
//        translate([0,0,wallThickness])
//            roundedCuboid(width=width-wallThickness,
//                          length=width-wallThickness,
//                          height=height+radius+1,
//                          radius=radius-wallThickness);
//        translate([0,0,height/2])
//           cube(size=[width,
//                      batWidth+batGap,
//                      2*height], center=true);
//    }
//}
//
////Battery opening
//module batOpening() {
//    translate([0,0,height-batHeight-compHeight])
//        roundedCuboid(width=width+2*radius+2,
//                    length=batWidth,
//                    height=height+radius+1,
//                    radius=radius);
//}
//
////Screw holes
//module screwHoles() {
//    for (rotation=[0,90,180,270]) {
//        rotate([0,0,rotation])
//            translate([18,18,-1])
//                cylinder(h=height+2, 
//                         r=screwRadius, 
//                         center=false);
//        rotate([0,0,rotation])
//            translate([18,18,-1])
//                rotate([0,0,-rotation])
//                    cylinder(h=1+height-screwLength-wallThickness, 
//                             r=nutRadius, 
//                             center=false,
//                             $fn=6);
//        rotate([0,0,rotation])
//            translate([18,18,height-screwLength-wallThickness])
//                rotate([0,0,-rotation])
//                     cylinder(h=((nutRadius-screwRadius)
//                                 *overhangIncline), 
//                             r1=nutRadius, 
//                             r2=screwRadius, 
//                             center=false,
//                             $fn=6);
//    }
//}
//
//}
//
////Component space
//module compSpace() {
//    difference() {
//        intersection() {
//            roundedCuboid(width=width-(2*wallThickness),
//                          length=width-(2*wallThickness),
//                          height=2*height,
//                          radius=radius-wallThickness);    
//            translate([0,0,height])
//                cube(size=[width+2,
//                           width+2,
//                           2*compHeight], center=true);
//        }
//        screwColumns();
//    }  
//
// }
//
////Logo
// module logo() {
//    for (rotation=[0,180]) {
//     rotate([0,0,rotation])
//         translate([0,-width/2,height/2])
//            rotate([90,0,0])
//                linear_extrude(height=2*engraveDepth, slices=2, center=true)
//                    text("MagniCube",
//                         valign="center",
//                         halign="center",
//                         size=height/4,
//                         font="Tw Cen MT Condensed Extra Bold:style=Regular");
//    }
// }
//
////outerBody();
////heightLimit();
////batSpace();
////batOpening();
////screwHoles();
////screwColumns();
////compSpace();
////logo();
// 
////Base
//module base() {
//    difference() {
//        //Positives
//        union() {
//            //Outer body
//            outerBody();
//            //Screw columns
//            screwColumns();
//        }
//        //Negatives
//        union() {
//            //Height limit
//            heightLimit();
//            //Battery space
//            batSpace();
//            //Battery opening
//            batOpening();
//            //Screw holes
//            screwHoles();
//            //Component space
//            compSpace();
//            //Logo
//            logo();
//        }
//    }
//};
//
//base();
