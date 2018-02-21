#define LIDAR_ADDR 0x52

#define USE_AND_OR	// To enable AND_OR mask setting for I2C. 


// Wait by executing nops

void i2c_wait(unsigned int cnt) {
    while (--cnt) {
        asm( "nop");
        asm( "nop");
    }
}

// Write a number of registers from data specified by num to the specified address
// logic taken from balancing robot project -- is slightly modified for VL53L0X

void i2c_write(char index, char data, int num) {
    char i2c_header[2];
    i2c_header[0] = LIDAR_ADDR | 0; //device address & WR
    i2c_header[1] = index; //register address

    StartI2C1(); //Send the Start Bit
    IdleI2C1(); //Wait to complete

    int i;
    for (i = 0; i < num + 2; i++) {
        if (i < 2)
            MasterWriteI2C1(i2c_header[i]);
        else
            MasterWriteI2C1(data);
        IdleI2C1(); //Wait to complete

        //ACKSTAT is 0 when slave acknowledge, 
        //if 1 then slave has not acknowledge the data.
        if (I2C1STATbits.ACKSTAT)
            break;
    }

    StopI2C1(); //Send the Stop condition
    IdleI2C1(); //Wait to complete
}

//modified version of i2c_write to handle 16bit writes 

void i2c_write_16(char index, short data, int num) {
    char i2c_header[2];
    i2c_header[0] = LIDAR_ADDR | 0; //device address & WR
    i2c_header[1] = index; //register address

    StartI2C1(); //Send the Start Bit
    IdleI2C1(); //Wait to complete

    int i;
    for (i = 0; i < num + 2; i++) {
        if (i < 2)
            MasterWriteI2C1(i2c_header[i]);
        else if (i == 2)
            MasterWriteI2C1((data >> 8) & 0xFF);
        else
            MasterWriteI2C1(data & 0xFF);
        IdleI2C1(); //Wait to complete

        //ACKSTAT is 0 when slave acknowledge, 
        //if 1 then slave has not acknowledge the data.
        if (I2C1STATbits.ACKSTAT)
            break;
    }

    StopI2C1(); //Send the Stop condition
    IdleI2C1(); //Wait to complete
}

//modified version of i2c_write to handle 32bit writes 

void i2c_write_32(char index, int data, int num) {
    char i2c_header[2];
    i2c_header[0] = LIDAR_ADDR | 0; //device address & WR
    i2c_header[1] = index; //register address

    StartI2C1(); //Send the Start Bit
    IdleI2C1(); //Wait to complete

    int i;
    for (i = 0; i < num + 2; i++) {
        if (i < 2)
            MasterWriteI2C1(i2c_header[i]);
        else if (i == 2)
            MasterWriteI2C1((data >> 24) & 0xFF);
        else if (i == 3)
            MasterWriteI2C1((data >> 16) & 0xFF);
        else if (i == 4)
            MasterWriteI2C1((data >> 8) & 0xFF);
        else
            MasterWriteI2C1(data & 0xFF);
        IdleI2C1(); //Wait to complete

        //ACKSTAT is 0 when slave acknowledge, 
        //if 1 then slave has not acknowledge the data.
        if (I2C1STATbits.ACKSTAT)
            break;
    }

    StopI2C1(); //Send the Stop condition
    IdleI2C1(); //Wait to complete
}

void i2c_write_multi_reg(char address, char * source, int num_of_regs) {
    char i2c_header[2];
    i2c_header[0] = LIDAR_ADDR | 0; //device address & WR
    i2c_header[1] = address; //register address

    StartI2C1(); //Send the Start Bit
    IdleI2C1(); //Wait to complete

    int i;
    for (i = 0; i < num_of_regs + 2; i++) {
        if (i < 2)
            MasterWriteI2C1(i2c_header[i]);
        else
            while (num_of_regs-- > 0) {
                MasterWriteI2C1(*(source++));
            }

        IdleI2C1(); //Wait to complete

        //ACKSTAT is 0 when slave acknowledge, 
        //if 1 then slave has not acknowledge the data.
        if (I2C1STATbits.ACKSTAT)
            break;
    }

    StopI2C1(); //Send the Stop condition
    IdleI2C1(); //Wait to complete
}

char i2c_read(char address) {
    char i2c_header[2];
    i2c_header[0] = (LIDAR_ADDR | 0); //device address & WR
    i2c_header[1] = address; //register address

    StartI2C1(); //Send the Start Bit
    IdleI2C1(); //Wait to complete



    int i;
    for (i = 0; i < 2; i++) {
        MasterWriteI2C1(i2c_header[i]);
        IdleI2C1(); //Wait to complete
        //printf("here\r\n");
        //ACKSTAT is 0 when slave acknowledge, 
        //if 1 then slave has not acknowledge the data.
        if (I2C1STATbits.ACKSTAT) {
            break;
        }
    }

    //now send a start sequence again
    RestartI2C1(); //Send the Restart condition
    i2c_wait(10);
    //wait for this bit to go back to zero
    IdleI2C1(); //Wait to complete

    MasterWriteI2C1(LIDAR_ADDR | 1); //transmit read command
    IdleI2C1(); //Wait to complete

    // read some bytes back
    //	MastergetsI2C1(num, dataBuf, 20);
    char data = MasterReadI2C1();
    //printf("%x\r\n", data);
    IdleI2C1(); //Wait to complete

    StopI2C1(); //Send the Stop condition
    IdleI2C1(); //Wait to complete

    return data;
}

short i2c_read_16(char address) {
    //("here\r\n");
    char i2c_header[2];
    i2c_header[0] = (LIDAR_ADDR | 0); //device address & WR
    i2c_header[1] = address; //register address

    StartI2C1(); //Send the Start Bit
    IdleI2C1(); //Wait to complete



    int i;
    for (i = 0; i < 2; i++) {
        MasterWriteI2C1(i2c_header[i]);
        IdleI2C1(); //Wait to complete
        //printf("here\r\n");
        //ACKSTAT is 0 when slave acknowledge, 
        //if 1 then slave has not acknowledge the data.
        if (I2C1STATbits.ACKSTAT) {
            break;
        }
    }

    //now send a start sequence again
    RestartI2C1(); //Send the Restart condition
    i2c_wait(10);
    //wait for this bit to go back to zero
    IdleI2C1(); //Wait to complete

    MasterWriteI2C1(LIDAR_ADDR | 1); //transmit read command
    IdleI2C1(); //Wait to complete

    // read some bytes back
    //	MastergetsI2C1(num, dataBuf, 20);
    short data = MasterReadI2C1() << 8;
    //printf("%d\r\n", data);
    IdleI2C1(); //Wait to complete

    data |= MasterReadI2C1();
    //printf("%d\r\n", data);
    IdleI2C1(); //wait to complete

    StopI2C1(); //Send the Stop condition
    IdleI2C1(); //Wait to complete

    return data;
}

//logic taken from balancing robot project
// Read a char from the register specified by address

void i2c_read_multi_reg(char address, char * destination, int num_of_regs) {
    char i2c_header[2];
    i2c_header[0] = (LIDAR_ADDR | 0); //device address & WR
    i2c_header[1] = address; //register address

    StartI2C1(); //Send the Start Bit
    IdleI2C1(); //Wait to complete



    int i;
    for (i = 0; i < 2; i++) {
        MasterWriteI2C1(i2c_header[i]);
        IdleI2C1(); //Wait to complete
        //printf("here\r\n");
        //ACKSTAT is 0 when slave acknowledge, 
        //if 1 then slave has not acknowledge the data.
        if (I2C1STATbits.ACKSTAT) {
            break;
        }
    }

    //now send a start sequence again
    RestartI2C1(); //Send the Restart condition
    i2c_wait(10);
    //wait for this bit to go back to zero
    IdleI2C1(); //Wait to complete

    MasterWriteI2C1(LIDAR_ADDR | 1); //transmit read command
    IdleI2C1(); //Wait to complete


    while (num_of_regs-- > 0) {
        *(destination++) = MasterReadI2C1();
        IdleI2C1(); //wait to complete
    }

    StopI2C1();
    IdleI2C1();

    //return data;
}