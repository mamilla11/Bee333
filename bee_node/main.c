#include <xdc/std.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/display/Display.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC26XX.h>

#include "Board.h"

#include "NodeRadioTask.h"
#include "NodeTask.h"

int main(void)
{
    board_init();

    NodeRadioTask_init();
    NodeTask_init();

    BIOS_start();

    return (0);
}
