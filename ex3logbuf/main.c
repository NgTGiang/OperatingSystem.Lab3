#include "logger.h"
#include <unistd.h>

int main() {
    init_logger();
    
    // Example log writes
    wrlog("Event 1");
    wrlog("Event 2");
    wrlog("Event 3");
    wrlog("Event 4");
    wrlog("Event 5"); // Fills buffer
    
    // This will block until flush occurs
    wrlog("Event 6");
    
    sleep(3); // Wait for automatic flush
    
    // Manual flush
    flushlog();
    
    return 0;
}