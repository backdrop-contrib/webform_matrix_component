/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

(function ($) {

	show_hide_element_type = function(elementid){

		var $type=$('#edit-extra-element-'+elementid+'-type').val();
		switch($type){
			case 'textfield': 
			case 'date':    
				$('.parent-form-item-extra-element-'+elementid+'-label-name').hide();
				$('.parent-form-item-extra-element-'+elementid+'-option').hide();
				break;

			case 'select':
				var label_div='form-item-extra-element-'+elementid+'-label-name';
				console.log(label_div+' #form-item-element-element-1-label-name');
				$('.parent-form-item-extra-element-'+elementid+'-option').show();
				$('.parent-form-item-extra-element-'+elementid+'-label-name').hide();
				break;

			case 'label':
				$('.parent-form-item-extra-element-'+elementid+'-label-name').show();
				$('.parent-form-item-extra-element-'+elementid+'-option').hide();
				break;
		}
    }
}) (jQuery);