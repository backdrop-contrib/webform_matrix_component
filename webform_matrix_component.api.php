<?php

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * validate hook for matrix component
 * you can define custom validation for this
 * @param type $element
 * @param type $form_state
 * 
 */
function hook_webform_matrix_validate($element, $form_state) {
    
}

/**
 * Validate textfield Numeric validation
 * @see webform_render_matrix_textfield_Numeric
 */
function webform_render_matrix_textfield_Numeric($element, $form_state) {
    $value = $element['#value'];
    if (!empty($value)) {
        if (!is_numeric($value)) {
            form_error($element, t('Only Numeric value is allowed'));
        }
    }
}

/**
 * Validate textfield Numeric validation
 * @see webform_render_matrix_textfield_Numeric
 */
function webform_render_matrix_textfield_Alphabet($element, $form_state) {
    
}

/**
 * webform_edit_matrix_alter for alter matrix edit form
 * like add helptext textfield
 * @see hook_webform_render_matrix_alter
 * @param type $element
 * @param type $element_values
 * @param type $element_id
 * 
 */
function hook_webform_edit_matrix_alter(&$element, &$element_values, $element_id) {
    $helptext = $element_values['helptext'];
    $element['helptext'] = array(
        '#type' => 'textfield',
        '#title' => 'Help Text',
        '#size' => 15,
        '#default_value' => $helptext,
        '#parents' => array('extra', 'element', $element_id, 'helptext'),
        '#weight' => 1,
    );
}

/**
 * webform_render_matrix_alter for alter webform_render_matrix 
 * @see hook_webform_edit_matrix_alter
 * @param type $component
 * @param type $value
 * @param type $element
 */
function hook_webform_render_matrix_alter(&$component, &$value, &$element) {
    $helptext = $value['helptext'];
    if (!empty($helptext)) {
        $element['#field_suffix'] = $helptext;
    }
}


?>


