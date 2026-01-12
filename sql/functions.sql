CREATE OR REPLACE FUNCTION getOrderStatus(p_order_id INT)
RETURNS VARCHAR AS $$
BEGIN
    RETURN (SELECT status FROM orders WHERE id = p_order_id);
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION getUserOrderCount(p_user_id INT)
RETURNS INT AS $$
BEGIN
    RETURN (SELECT COUNT(*) FROM orders WHERE user_id = p_user_id);
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION getTotalSpentByUser(p_user_id INT)
RETURNS DECIMAL AS $$
BEGIN
    RETURN COALESCE((SELECT SUM(total_amount) FROM orders WHERE user_id = p_user_id), 0);
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION canReturnOrder(p_order_id INT)
RETURNS BOOLEAN AS $$
BEGIN
    RETURN (
        SELECT status = 'completed' AND 
               (CURRENT_TIMESTAMP - created_at) < INTERVAL '30 days'
        FROM orders 
        WHERE id = p_order_id
    );
END;
$$ LANGUAGE plpgsql;
