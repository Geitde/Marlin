/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(SWITCHING_TOOLHEAD_GEITPRINTER)  /* we should do a proper define for this in the future */

#include "menu.h"
#include "../../module/probe.h"
#include "../../module/motion.h"
#include "../../module/planner.h"
#include "../../module/temperature.h"

/*
** internal function prototypes
*/ 

static void gp_toolhead_park( unsigned int toolhead );
static void gp_toolhead_park_t1();
  #if EXTRUDERS > 1
static void gp_toolhead_park_t2();
  #endif
  #if EXTRUDERS > 2
static void gp_toolhead_park_t3();
  #endif
  #if EXTRUDERS > 3
static void gp_toolhead_park_t4();
  #endif
static void gp_toolhead_grab( unsigned int toolhead );
static void gp_toolhead_grab_t1();
  #if EXTRUDERS > 1
static void gp_toolhead_grab_t2();
  #endif
  #if EXTRUDERS > 2
static void gp_toolhead_grab_t3();
  #endif
  #if EXTRUDERS > 3
static void gp_toolhead_grab_t4();
  #endif
static void gp_toolhead_mount( unsigned int toolhead );
static void gp_toolhead_mount_t1();
  #if EXTRUDERS > 1
static void gp_toolhead_mount_t2();
  #endif
  #if EXTRUDERS > 2
static void gp_toolhead_mount_t3();
  #endif
  #if EXTRUDERS > 3
static void gp_toolhead_mount_t4();
  #endif

static void menu_geitprinter_setup_toolhead( unsigned int toolhead );

/***************************************************************************************************************/

#define BACK_ITEM_P(LABEL)                                MENU_ITEM_P(back, LABEL)

/***************************************************************************************************************/


static void menu_geitprinter_setup_toolhead( unsigned int toolhead ) {

    auto _recalc_offsets0 = []{
      if (active_extruder == 0 && all_axes_known()) {  // For the 2nd extruder re-home so the next tool-change gets the new offsets.
        queue.inject_P(G28_STR); // In future, we can babystep the 2nd extruder (if active), making homing unnecessary.
        //active_extruder = 0; /* why?? (geit) */
      }
    };
  #if EXTRUDERS > 1
    auto _recalc_offsets1 = []{
      if (active_extruder == 1 && all_axes_known()) {  // For the 2nd extruder re-home so the next tool-change gets the new offsets.
        queue.inject_P(G28_STR); // In future, we can babystep the 2nd extruder (if active), making homing unnecessary.
        //active_extruder = 0; /* why?? (geit) */
      }
    };
  #endif
  #if EXTRUDERS > 2
    auto _recalc_offsets2 = []{
      if (active_extruder == 2 && all_axes_known()) {  // For the 2nd extruder re-home so the next tool-change gets the new offsets.
        queue.inject_P(G28_STR); // In future, we can babystep the 2nd extruder (if active), making homing unnecessary.
        //active_extruder = 0; /* why?? (geit) */
      }
    };
  #endif
  #if EXTRUDERS > 3
    auto _recalc_offsets3 = []{
      if (active_extruder == 3 && all_axes_known()) {  // For the 2nd extruder re-home so the next tool-change gets the new offsets.
        queue.inject_P(G28_STR); // In future, we can babystep the 2nd extruder (if active), making homing unnecessary.
        //active_extruder = 0; /* why?? (geit) */
      }
    };
  #endif
  START_MENU();
  BACK_ITEM_P( "Toolhead Setup" );
    switch( toolhead ) {
        case 0:
            EDIT_ITEM_FAST_P(float42_52, "SETUP T1 X", &hotend_offset[ toolhead ].x, -99.0, 99.0, _recalc_offsets0);
            EDIT_ITEM_FAST_P(float42_52, "SETUP T1 Y", &hotend_offset[ toolhead ].y, -99.0, 99.0, _recalc_offsets0);
            EDIT_ITEM_FAST_P(float42_52, "SETUP T1 Z", &hotend_offset[ toolhead ].z, Z_PROBE_LOW_POINT, 10.0, _recalc_offsets0);
            ACTION_ITEM_P(PSTR("Mount Separator T1"), gp_toolhead_mount_t1);
            break;
  #if EXTRUDERS > 1
        case 1:
            EDIT_ITEM_FAST_P(float42_52, "SETUP T2 X", &hotend_offset[ toolhead ].x, -99.0, 99.0, _recalc_offsets1);
            EDIT_ITEM_FAST_P(float42_52, "SETUP T2 Y", &hotend_offset[ toolhead ].y, -99.0, 99.0, _recalc_offsets1);
            EDIT_ITEM_FAST_P(float42_52, "SETUP T2 Z", &hotend_offset[ toolhead ].z, Z_PROBE_LOW_POINT, 10.0, _recalc_offsets1);
            ACTION_ITEM_P(PSTR("Mount Separator T2"), gp_toolhead_mount_t2);
            break;
  #endif
  #if EXTRUDERS > 2
        case 2:
            EDIT_ITEM_FAST_P(float42_52, "SETUP T3 X", &hotend_offset[ toolhead ].x, -99.0, 99.0, _recalc_offsets2);
            EDIT_ITEM_FAST_P(float42_52, "SETUP T3 Y", &hotend_offset[ toolhead ].y, -99.0, 99.0, _recalc_offsets2);
            EDIT_ITEM_FAST_P(float42_52, "SETUP T3 Z", &hotend_offset[ toolhead ].z, Z_PROBE_LOW_POINT, 10.0, _recalc_offsets2);
            ACTION_ITEM_P(PSTR("Mount Separator T3"), gp_toolhead_mount_t3);
            break;
  #endif
  #if EXTRUDERS > 3
        case 3:
            EDIT_ITEM_FAST_P(float42_52, "SETUP T4 X", &hotend_offset[ toolhead ].x, -99.0, 99.0, _recalc_offsets3);
            EDIT_ITEM_FAST_P(float42_52, "SETUP T4 Y", &hotend_offset[ toolhead ].y, -99.0, 99.0, _recalc_offsets3);
            EDIT_ITEM_FAST_P(float42_52, "SETUP T4 Z", &hotend_offset[ toolhead ].z, Z_PROBE_LOW_POINT, 10.0, _recalc_offsets3);
            ACTION_ITEM_P(PSTR("Mount Separator T4"), gp_toolhead_mount_t4);
            break;
  #endif
    }
  END_MENU();
}


/* /// menu_geitprinter_setup 
**
*/

/*****************************************************************************************/

static void menu_geitprinter_setup_t1() {
    menu_geitprinter_setup_toolhead( 0 );
}
  #if EXTRUDERS > 1
static void menu_geitprinter_setup_t2() {
    menu_geitprinter_setup_toolhead( 1 );
}
  #endif
  #if EXTRUDERS > 2
static void menu_geitprinter_setup_t3() {
    menu_geitprinter_setup_toolhead( 2 );
}
  #endif
  #if EXTRUDERS > 3
static void menu_geitprinter_setup_t4() {
    menu_geitprinter_setup_toolhead( 3 );
}
  #endif

/*****************************************************************************************/

static void menu_geitprinter_setup() {
  START_MENU();
  BACK_ITEM_P( "Geit Printer" );
    SUBMENU_P(PSTR("Setup T1"), menu_geitprinter_setup_t1 );
  #if EXTRUDERS > 1
    SUBMENU_P(PSTR("Setup T2"), menu_geitprinter_setup_t2 );
  #endif
  #if EXTRUDERS > 2
    SUBMENU_P(PSTR("Setup T3"), menu_geitprinter_setup_t3 );
  #endif
  #if EXTRUDERS > 3
    SUBMENU_P(PSTR("Setup T4"), menu_geitprinter_setup_t4 );
  #endif
  END_MENU();
}

/* /// menu_geitprinter_park()
**
*/

/*****************************************************************************************/

static void menu_geitprinter_park() {
  START_MENU();
  BACK_ITEM_P( "Geit Printer" );
    ACTION_ITEM_P(PSTR("Park T1"), gp_toolhead_park_t1 );
  #if EXTRUDERS > 1
    ACTION_ITEM_P(PSTR("Park T2"), gp_toolhead_park_t2 );
  #endif
  #if EXTRUDERS > 2
    ACTION_ITEM_P(PSTR("Park T3"), gp_toolhead_park_t3 );
  #endif
  #if EXTRUDERS > 3
    ACTION_ITEM_P(PSTR("Park T4"), gp_toolhead_park_t4 );
  #endif
  END_MENU();
}
/* \\\ */

/* /// menu_geitprinter_grab()
**
*/

/*****************************************************************************************/

static void menu_geitprinter_grab() {
  START_MENU();
  BACK_ITEM_P( "Geit Printer" );
    ACTION_ITEM_P(PSTR("Grab T1"), gp_toolhead_grab_t1);
  #if EXTRUDERS > 1
    ACTION_ITEM_P(PSTR("Grab T2"), gp_toolhead_grab_t2);
  #endif
  #if EXTRUDERS > 2
    ACTION_ITEM_P(PSTR("Grab T3"), gp_toolhead_grab_t3);
  #endif
  #if EXTRUDERS > 3
    ACTION_ITEM_P(PSTR("Grab T4"), gp_toolhead_grab_t4);
  #endif
  END_MENU();
}
/* \\\ */

/* /// menu_geitprinter()
**
*/

/*****************************************************************************************/

void menu_geitprinter() {
  START_MENU();
  BACK_ITEM_P( "Main Menu");
    SUBMENU_P(PSTR("Toolhead Park"), menu_geitprinter_park);
    SUBMENU_P(PSTR("Toolhead Grab"), menu_geitprinter_grab);
    SUBMENU_P(PSTR("Toolhead Setup"), menu_geitprinter_setup);
  END_MENU();
}



/*
*****************************************************************************************************************
*
* geitprinter functions
*
*/

/* /// gp_toolhead_park()
**
*/

/*****************************************************************************************/

static void gp_toolhead_park_t1() {
    gp_toolhead_park( 0 );
}
  #if EXTRUDERS > 1
static void gp_toolhead_park_t2() {
    gp_toolhead_park( 1 );
}
  #endif
  #if EXTRUDERS > 2
static void gp_toolhead_park_t3() {
    gp_toolhead_park( 2 );
}
  #endif
  #if EXTRUDERS > 3
static void gp_toolhead_park_t4() {
    gp_toolhead_park( 3 );
}
  #endif

/*****************************************************************************************/

static void gp_toolhead_park( unsigned int toolhead )
{

    constexpr float toolheadposx[] = GPTC_TOOLHEAD_DOCKINGBAY_X_POS,
                    toolheadposy   = GPTC_TOOLHEAD_DOCKINGBAY_Y_POS;
    constexpr float dockingdiffx[] = GPTC_TOOLHEAD_MOVEMENT_DOCKING_X,
                    dockingdiffy[] = GPTC_TOOLHEAD_MOVEMENT_DOCKING_Y,
                    dockingfeed[]  = GPTC_TOOLHEAD_MOVEMENT_DOCKING_F;
    unsigned int i;
//    #define UIBUFFER_SIZEOF 0x40
//    char uibuffer[ UIBUFFER_SIZEOF ];

    for( i = 0 ; dockingfeed[ i ] ; i++ ) {
        current_position.x = toolheadposx[ toolhead ]  + dockingdiffx[ i ];
        current_position.y = toolheadposy                     + dockingdiffy[ i ];
//        sprintf_P( uibuffer, "Dock: T%d: %d/%d", toolhead, (int) current_position.x, (int) current_position.y );
//        ui.set_status( uibuffer );
        line_to_current_position( MMM_TO_MMS( dockingfeed[ i ] ) );
    }

    planner.synchronize();        // Always sync the final move
    ui.set_status( "Docking complete!" );

}
/* \\\ */

/* /// gp_toolhead_grab()
**
*/

/*****************************************************************************************/

static void gp_toolhead_grab_t1() {
    gp_toolhead_grab( 0 );
}
  #if EXTRUDERS > 1
static void gp_toolhead_grab_t2() {
    gp_toolhead_grab( 1 );
}
  #endif
  #if EXTRUDERS > 2
static void gp_toolhead_grab_t3() {
    gp_toolhead_grab( 2 );
}
  #endif
  #if EXTRUDERS > 3
static void gp_toolhead_grab_t4() {
    gp_toolhead_grab( 3 );
}
  #endif

/*****************************************************************************************/

static void gp_toolhead_grab( unsigned int toolhead )
{

    constexpr float toolheadposx[] = GPTC_TOOLHEAD_DOCKINGBAY_X_POS,
                    toolheadposy   = GPTC_TOOLHEAD_DOCKINGBAY_Y_POS;
    constexpr float undockingdiffx[] = GPTC_TOOLHEAD_MOVEMENT_UNDOCKING_X,
                    undockingdiffy[] = GPTC_TOOLHEAD_MOVEMENT_UNDOCKING_Y,
                    undockingfeed[]  = GPTC_TOOLHEAD_MOVEMENT_UNDOCKING_F;
    unsigned int i;
//    #define UIBUFFER_SIZEOF 0x40
//    char uibuffer[ UIBUFFER_SIZEOF ];


    for( i = 0 ; undockingfeed[ i ] ; i++ ) {
        current_position.x = toolheadposx[ toolhead ]         + undockingdiffx[ i ];
        current_position.y = toolheadposy                     + undockingdiffy[ i ];
//        sprintf_P( uibuffer, "Undock: T%d: %d/%d", toolhead, (int) current_position.x, (int) current_position.y );
//        ui.set_status( uibuffer );
        line_to_current_position( MMM_TO_MMS( undockingfeed[ i ] ) );
    }

    planner.synchronize();        // Always sync the final move
    ui.set_status( "Undocking complete!" );

}
/* \\\ */


/* /// gp_toolhead_mount()
**
** WARNING: This command must only be executed when the parking area and the tool head is 
** empty or it will end in a gigantic disaster.
**
** The specified printhed will drive to the print safe zone and from there direct towards
** the far most Y position of that slot.
**
** This function is ment to replace a broken tool separator. Once the toolhead finished
** moving move the tool separator towards the magnetic holder and tighen one of the screws
** push the button and tighen the other screw.
**
*/

/*****************************************************************************************/

static void gp_toolhead_mount_t1() {
   gp_toolhead_mount( 0 );
}
  #if EXTRUDERS > 1
static void gp_toolhead_mount_t2() {
   gp_toolhead_mount( 1 );
}
  #endif
  #if EXTRUDERS > 2
static void gp_toolhead_mount_t3() {
   gp_toolhead_mount( 2 );
}
  #endif
  #if EXTRUDERS > 3
static void gp_toolhead_mount_t4() {
   gp_toolhead_mount( 3 );
}
  #endif

/*****************************************************************************************/

static void gp_toolhead_mount( unsigned int toolhead )
{

    constexpr float toolheadposx[] = GPTC_TOOLHEAD_DOCKINGBAY_X_POS,
                    toolheadposy   = GPTC_TOOLHEAD_DOCKINGBAY_Y_POS;
    constexpr float dockingdiffy[] = GPTC_TOOLHEAD_MOVEMENT_DOCKING_Y,
                    dockingfeed[]  = GPTC_TOOLHEAD_MOVEMENT_DOCKING_F;
    unsigned int i, maxy;
              float old_x = current_position.x;
              float old_y = current_position.y;

    #define UIBUFFER_SIZEOF 0x40
    char uibuffer[ UIBUFFER_SIZEOF ];


    if ( (IsRunning() && !axis_unhomed_error( X_AXIS | Y_AXIS | Z_AXIS ) ) ) {
        /* find the maximum y offset for docking from docking array */
        maxy = 0;
        for( i = 0 ; dockingfeed[ i ] ; i++ ) {
            if( dockingdiffy[ i ] > maxy ) {
                maxy = dockingdiffy[ i ];
            }
        }

     /* are we save to proceed? */
        if( current_position.y > toolheadposy ) {
        #if 0
            ui.set_status( "leaving dock" );
            current_position.y = toolheadposy; /* go straight out of the docking area */
            line_to_current_position( MMM_TO_MMS( GPTC_TOOLHEAD_DOCKINGBAY_TRAVEL / 10 ) ); /* slowly */
            old_y = toolheadposy; /* make sure we do not race into the docking ports at the end */
        #else
            ui.set_status( "head inside dock" );
            return;
        #endif
    }
     /* go straight into the docking area safe zone */
        ui.set_status( "aproaching safe zone" );
        current_position.y = toolheadposy; /* go straight into the docking area safe zone */
        line_to_current_position( MMM_TO_MMS( GPTC_TOOLHEAD_DOCKINGBAY_TRAVEL ) );

     /* approaching dock */
        sprintf_P( uibuffer, "aproaching dock %d", toolhead );
        ui.set_status( uibuffer );
        current_position.x = toolheadposx[ toolhead ]; /* we need the plain extruder parking offset */
        current_position.y = toolheadposy;
        line_to_current_position( MMM_TO_MMS( GPTC_TOOLHEAD_DOCKINGBAY_TRAVEL ) );

     /* moving into the dock */
        sprintf_P( uibuffer, "entering dock %d", toolhead );
        ui.set_status( uibuffer );
        current_position.x = toolheadposx[ toolhead ]; /* we need the plain extruder parking offset */
        current_position.y = toolheadposy + maxy;
        line_to_current_position( MMM_TO_MMS( GPTC_TOOLHEAD_DOCKINGBAY_TRAVEL / 10 ) );

     /* button wait */

        wait_for_user = true;
        while( wait_for_user ) {
           idle_no_sleep();
        }
        wait_for_user = false; // just in case we add additional abort cases

     /* leaving dock */
        sprintf_P( uibuffer, "leaving dock %d", toolhead );
        ui.set_status( uibuffer );
        current_position.x = toolheadposx[ toolhead ]; /* we need the plain extruder parking offset */
        current_position.y = toolheadposy;
        line_to_current_position( MMM_TO_MMS( GPTC_TOOLHEAD_DOCKINGBAY_TRAVEL / 10 ) );

    /* returning to previous position */
        ui.set_status( "return to base" );
        current_position.x = old_x;
        current_position.y = old_y;
        line_to_current_position( MMM_TO_MMS( GPTC_TOOLHEAD_DOCKINGBAY_TRAVEL ) );

        planner.synchronize();        // Always sync the final move
        ui.set_status( "operation done" );

    } /* moving before homing */

}
/* \\\ */

/*****************************************************************************************/

#endif // SWITCHING_TOOLHEAD_GEITPRINTER
