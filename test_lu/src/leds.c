#include "leds.h"


void sig_handler(int signum)
{
	int g_stop;
	switch( signum )
	{
		case SIGINT:
		case SIGTERM:
			g_stop = 1;

		default:
			break;
	}

	return ;
}



int term_led(leds_t *leds)
{
	int            i;
	led_t         *led;

	printf("terminate RGB Led gpios\n");

	if( !leds )
	{
		printf("Invalid input arguments\n");
		return -1;
	}

	for(i=0; i<leds->count; i++)
	{
		led = &leds->leds[i];

		if( led->request )
		{
			turn_led(leds, i, OFF);
			gpiod_line_request_release(led->request);
		}
	}

	return 0;
}



int init_led(leds_t *leds)
{
	led_t                       *led;
	int                          i, rv = 0;
	char                         chip_dev[32];
	struct gpiod_chip           *chip;      /* gpio chip */
	struct gpiod_line_settings  *settings;  /* gpio direction, bias, active_low, value */
	struct gpiod_line_config    *line_cfg;  /* gpio line */
	struct gpiod_request_config *req_cfg;   /* gpio consumer, it can be NULL */


	if( !leds )
	{
		printf("Invalid input arguments\n");
		return -1;
	}


	settings = gpiod_line_settings_new();
	if (!settings)
	{
		printf("unable to allocate line settings\n");
		rv = -2;
		goto cleanup;

	}


	line_cfg = gpiod_line_config_new();
	if( !line_cfg )
	{

		printf ("unable to allocate the line config structure\n");
		rv = -2;
		goto cleanup;
	}

	req_cfg = gpiod_request_config_new();
	if( !req_cfg )
	{

		printf ("unable to allocate the request config structure\n");
		rv = -2;
		goto cleanup;
	}


	for(i=0; i<leds->count; i++)
	{
		led = &leds->leds[i];

		snprintf(chip_dev, sizeof(chip_dev), "/dev/gpiochip%d", led->chip_num);
		chip = gpiod_chip_open(chip_dev);
		if( !chip )
		{

			printf ("open gpiochip failure, maybe you need running as root\n");
			rv = -3;
			goto cleanup;
		}

		/* Set as outputdirection, active low and default level as inactive */
		gpiod_line_settings_reset(settings);
		gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
		gpiod_line_settings_set_active_low(settings, led->active);
		gpiod_line_settings_set_output_value(settings, GPIOD_LINE_VALUE_INACTIVE);

		/* set gpio line */
		gpiod_line_config_reset(line_cfg);
		gpiod_line_config_add_line_settings(line_cfg, &led->gpio_num, 1, settings);

		/* can be NULL for default settings. */
		gpiod_request_config_set_consumer(req_cfg, led->name);

		/* request a set of lines for exclusive usage.*/
		led->request = gpiod_chip_request_lines(chip, req_cfg, line_cfg);

		gpiod_chip_close(chip);

	}

cleanup:
	if( rv<0 )
	{
		term_led(leds);
	}
	if( line_cfg )
	{
		gpiod_line_config_free(line_cfg);
	}
	if( req_cfg )
	{
		gpiod_request_config_free(req_cfg);
	}
	if( settings )
	{
		gpiod_line_settings_free(settings);
	}

	return rv;

}	 

static inline void msleep(unsigned long ms)
{   
	struct timespec cSleep;
	unsigned long   ulTmp;

	cSleep.tv_sec = ms / 1000;
	if (cSleep.tv_sec == 0)
	{   
		ulTmp = ms * 10000;
		cSleep.tv_nsec = ulTmp * 100;
	}       
	else
	{
		cSleep.tv_nsec = 0;
	}

	nanosleep(&cSleep, 0);

	return ;
}


int turn_led(leds_t *leds, int which, int cmd)
{
	led_t       *led;
	int          rv = 0;
	int          value = 0;

	if( !leds || which<0 || which>=leds->count )
	{

		printf ("Invalid input arguments\n");
		return -1;
	}



	led = &leds->leds[which];

	value = OFF == cmd ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE;

	gpiod_line_request_set_value(led->request, led->gpio_num, value);
	return 0;
}

int open_led(leds_t *leds, int which)
{
	led_t         *led;
	int            rv=0;

	if( !leds || which<0 || which >= leds->count)
	{

		printf ("Invalid input arguments\n");
		return -1;
	}


	led = &leds->leds[which];

	gpiod_line_request_set_value(led->request, led->gpio_num, ON);

	return 0;
}

int close_led(leds_t *leds, int which)
{
	led_t         *led;
	int            rv=0;

	if( !leds || which<0 || which >= leds->count)
	{   

		printf ("Invalid input arguments\n");
		return -1;
	}       



	led = &leds->leds[which];

	gpiod_line_request_set_value(led->request, led->gpio_num, OFF);

	return 0;
}

