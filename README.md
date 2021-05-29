# semaphores-FreeRTOS
In this project, there are 2 tasks - one is the ADC task where in it takes the analog value and other is the Ultrasonic task where in it calculates the obstacle distance. These two tasks are using a single resource to display the data that is LCD.  Semaphores are used to ensure that only one task at a time uses LCD to display the data. Using semaphore also ensures task synchronization. Firstly, ADC task acquires the semaphore, and displays the value on the LCD. Before the ADC task ends, it releases the semaphore. Now the control moves to ultrasonic task. It acquires the semaphore, calculates the distance and sends to the LCD to display.  Again, before the task ends, it releases the semaphore. The whole process repeats and it gives the real time data of ADC and ultrasonic sensor. At the software side, we have added libraries of FreeRTOS and semaphore using #include preprocessor directives. The coding part is done in Embedded C using Arduino IDE and the Software Simulation is done in ISIS Professional 8 Proteus.
