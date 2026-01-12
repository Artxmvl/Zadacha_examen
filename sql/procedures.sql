CREATE OR REPLACE PROCEDURE createOrder(
    p_user_id INT,
    p_product_id INT,
    p_quantity INT
)
LANGUAGE plpgsql
AS $$
DECLARE
    v_price DECIMAL;
    v_total DECIMAL;
    v_product_qty INT;
BEGIN
    SELECT price INTO v_price FROM products WHERE id = p_product_id;
    SELECT quantity INTO v_product_qty FROM products WHERE id = p_product_id;
    
    IF v_product_qty < p_quantity THEN
        RAISE EXCEPTION 'Not enough products in stock';
    END IF;
    
    v_total := v_price * p_quantity;
    
    INSERT INTO orders (user_id, total_amount) VALUES (p_user_id, v_total);
    INSERT INTO order_items (order_id, product_id, quantity, price) 
    VALUES (LASTVAL(), p_product_id, p_quantity, v_price);
    
    UPDATE products SET quantity = quantity - p_quantity WHERE id = p_product_id;
    
    INSERT INTO audit_log (user_id, action, details)
    VALUES (p_user_id, 'create_order', 'Order created for product ' || p_product_id);
    
    COMMIT;
EXCEPTION
    WHEN OTHERS THEN
        ROLLBACK;
        RAISE;
END;
$$;
