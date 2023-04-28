bool blinking = 0;
int blink_length = 200;
int led_pin = 13;
char input_buffer[64];
char encoded_buffer[256];
const char *morse_alfabet[26] = {
    "A.-",
    "B-...",
    "C-.-.",
    "D-..",
    "E.",
    "F..-.",
    "G--.",
    "H....",
    "I..",
    "J.---",
    "K-.-",
    "L.-..",
    "M--",
    "N-.",
    "O---",
    "P.--.",
    "Q--.-",
    "R.-.",
    "S...",
    "T-",
    "U..-",
    "V...-",
    "W.--",
    "X-..-",
    "Y-.--",
    "Z--.."
};

void morse_encode(char*, char*);
void send_morse(char*);
void send_output(int, int);

void setup() {
    Serial.begin(9600);
}

void loop() {
 	for (int i = 0; i < strlen(input_buffer); i++){
   		input_buffer[i] = ' ';
 	 }
  	for (int i = 0; i < strlen(encoded_buffer); i++){
    	encoded_buffer[i] = ' ';
  	}
  
  	while (Serial.available() == 0 || blinking){
  	}
  
    Serial.readString().toCharArray(input_buffer, 64);
    Serial.print("input: ");
	Serial.print(input_buffer);
  	Serial.print("\n");
  
    /*if (input_buffer[0] == '/'){
        switch (input_buffer){
            case "/ms":
                Serial.print("Enter ms\n");
                while (Serial.available() == 0)
                {
                }
                blink_length = Serial.read();
        }
        return;
    }
    */

    morse_encode(input_buffer, encoded_buffer);
    Serial.print("encoded string: ");
    Serial.print(encoded_buffer);
    Serial.print("\n");

}
void morse_encode(char* input_buffer, char* encoded_buffer) {
    int index = 0;
    for (int i = 0; i < strlen(input_buffer); i++) {
        //std::cout << "checking: " << input_buffer[i] << std::endl;
        if (input_buffer[i] == ' '){
            encoded_buffer[index] = '_';
            index +=1;
            continue;
        }
        
        char c = input_buffer[i] <= 'Z' ? input_buffer[i] : input_buffer[i] - ('a' - 'A');
        for (int j = 0; j < sizeof(morse_alfabet) / sizeof(*morse_alfabet); j++) {
            //std::cout << "iterating alfabet\t" << j << " : " << morse_alfabet[j] << std::endl;
            if (c == morse_alfabet[j][0]) {
                for (int k = 0; k < strlen(morse_alfabet[j])-1; k++) {
                    //std::cout << "encoding\t" << k << " : " << morse_alfabet[j][k+1] << std::endl;
                    encoded_buffer[index] = morse_alfabet[j][k + 1];
                    index += 1;
                }
                encoded_buffer[index] = ' ';
                index += 1;
                break;
            }
        }
    }
}

void send_morse(char* morse){
    for (int i = 0; i < strlen(morse); i++){
        switch (morse[i]){
    	    case '.':
                send_output(led_pin, blink_length);
            case '-':
                send_output(led_pin, blink_length*3);
            case ' ':
                send_output(led_pin, blink_length*3);
            case '_':
                send_output(led_pin, blink_length*7);
        }
    }
}

void send_output(int pin, int ms){
    digitalWrite(pin, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(ms);                       // wait for a second
    digitalWrite(pin, LOW);    // turn the LED off by making the voltage LOW
    delay(ms);
}
