/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

(function ($) {

    Drupal.behaviors.webform_matrix = Drupal.behaviors.webform_matrix || {};

    Drupal.behaviors.webform_matrix.attach = function(context) {
        // Calendar datepicker behavior.
        Drupal.webform_matrix.datepicker(context);
  
    };

    Drupal.webform_matrix = Drupal.webform_matrix || {};

    Drupal.webform_matrix.datepicker = function(context) {
	
        $('.webform-matrix-calendar').each(function() {
            var $calendar=this;
		
            var startDate = $calendar.className.replace(/.*webform-calendar-start-(\d{4}-\d{2}-\d{2}).*/, '$1').split('-');
            var endDate = $calendar.className.replace(/.*webform-calendar-end-(\d{4}-\d{2}-\d{2}).*/, '$1').split('-');
            var firstDay = $calendar.className.replace(/.*webform-calendar-day-(\d).*/, '$1');
            var defaultDay=$calendar.className.replace(/.*webform-calendar-defaultDay-(\d).*/, '$1');
            // Convert date strings into actual Date objects.
            startDate = new Date(startDate[0], startDate[1] - 1, startDate[2]);
            endDate = new Date(endDate[0], endDate[1] - 1, endDate[2]);

            // Ensure that start comes before end for datepicker.
            if (startDate > endDate) {
                var laterDate = startDate;
                startDate = endDate;
                endDate = laterDate;
            }

            var startYear = startDate.getFullYear();
            var endYear = endDate.getFullYear();

            // Set up the jQuery datepicker element.
            $(this).datepicker({
                dateFormat: 'yy-mm-dd',
                yearRange: startYear + ':' + endYear,
                firstDay: parseInt(firstDay),
                minDate: startDate,
                maxDate: endDate,
                changeMonth:true,
                changeYear:true,
                Default:defaultDay
            });
		  
            // Prevent the calendar button from submitting the form.
            $calendar.click(function(event) {
                $(this).focus();
                event.preventDefault();
            });
        });
    }
	
}) (jQuery);
