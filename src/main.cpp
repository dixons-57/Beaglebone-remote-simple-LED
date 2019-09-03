#include <unistd.h>
#include "iostream"
#include <stdio.h>
#include <dirent.h>

using namespace std;

int main()
{
	//file handle pointers
	FILE* device_export_file = nullptr;
	FILE* device_unexport_file = nullptr;
	FILE* pin48_direction_file = nullptr;
	FILE* pin_48_value_file = nullptr;

	//strings for convenience
	char lowString[] = "0";
	char highString[] = "1";
	char directionOutString[] = "out";
	char pinNumber[] = "48";

	bool shouldUnexportAfterwards = false;



	// EXPORT THE PIN

	/* only export gpio48 if we need to, as trying to do so if it is already
	//available causes all sorts of problems */
	DIR* gpio48Directory = opendir("/sys/class/gpio/gpio48");
	if (gpio48Directory != nullptr)
	{
		// "directory exists", so don't bother
		closedir(gpio48Directory);
		puts("No need to make gpio 48 available");
	}
	else if (ENOENT == errno) //doesn't exist already
	{
		//this part here exports gpio48
		device_export_file = fopen ("/sys/class/gpio/export", "w");
		fwrite (pinNumber, 1, sizeof(pinNumber), device_export_file);
		fclose (device_export_file);

		//remember that we exported it for the purpose of this program, it makes sense to put it back the way it was afterwards
		shouldUnexportAfterwards= true;
		puts("Made gpio 48 available, should make unavailable after");
	}
	else
	{
		return 0;
	}
	gpio48Directory = nullptr;
	fflush(NULL); //make sure stdout keeps up



	// SET THE PIN DIRECTION TO BE AN OUTPUT

	//wait until the pin "directory" has been created. sometimes there is a delay exposing the pin after exporting
	while(gpio48Directory == nullptr)
	{
		gpio48Directory = opendir("/sys/class/gpio/gpio48");
		usleep(100);
	}
	closedir(gpio48Directory);

	puts("Starting flashing");
	fflush(NULL);

	//this part here actually sets the direction of the pin, but we loop until the direction "file" has been created. sometimes there is a delay exposing full functionality of the pin
	while(pin48_direction_file == nullptr)
	{
		pin48_direction_file = fopen("/sys/class/gpio/gpio48/direction", "w");
		usleep(100);
	}




	// TOGGLE THE PIN ALTERNATELY HIGH AND LOW

	fwrite(directionOutString, 1, sizeof(directionOutString), pin48_direction_file); //write the high string to pin 48
	fclose(pin48_direction_file);
	usleep (250000);

	for (int i = 0; i < 5; i++)
	{
		pin_48_value_file = fopen ("/sys/class/gpio/gpio48/value", "w");
		fwrite (highString, 1, sizeof(highString), pin_48_value_file); //write the high string to pin 48
		fclose (pin_48_value_file);

		usleep (250000); //delay

		pin_48_value_file = fopen ("/sys/class/gpio/gpio48/value", "w");
		fwrite (lowString, 1, sizeof(lowString), pin_48_value_file); //write the low string to pin 48
		fclose (pin_48_value_file);

		usleep (250000); //delay
	}

	puts("Finished flashing");
	fflush(NULL);




	// MAKE PIN UNAVAILABLE AGAIN IF NECESSARY

	if(shouldUnexportAfterwards)
	{
		//this part here unexports the pin, putting the device tree state back to how it was
		device_unexport_file = fopen ("/sys/class/gpio/unexport", "w");
		fwrite (pinNumber, 1, sizeof(pinNumber), device_unexport_file);
		fclose (device_unexport_file);
		puts("Made gpio 48 unavailable");
		fflush(NULL);
	}

	return 0;
}
