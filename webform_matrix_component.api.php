<?php

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * validate hook for matrix component
 * @param type $element
 * @param type $form_state
 */
function hook_webform_matrix_validate($element, $form_state) {
    
}

/**
 * Validate textfield Numeric validation
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
 */
function webform_render_matrix_textfield_Alphabet($element, $form_state) {
    
}


?>
