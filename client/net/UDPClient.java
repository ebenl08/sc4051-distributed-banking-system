package client.net;

import java.net.SocketTimeoutException;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.TimeoutException;


import client.Constants;

public class UDPClient {

	private int semanticInvoc;
	private int requestIdCounter;
	private int timeoutMs;
	private int maxRetry;
	private final Map<Integer, Boolean> handledResponseIds;
	private boolean debug;

	public static final class ResponseDecision {
		public final boolean duplicate;
		public final boolean shouldAck;

		public ResponseDecision(boolean duplicate, boolean shouldAck) {
			this.duplicate = duplicate;
			this.shouldAck = shouldAck;
		}
	}

	public UDPClient() {
		this.semanticInvoc = Constants.NO_SEM_INVO;
		this.requestIdCounter = 0;
		this.timeoutMs = Constants.DEFAULT_TIMEOUT;
		this.maxRetry = Constants.DEFAULT_MAX_TIMEOUT;
		this.handledResponseIds = new HashMap<>();
		this.debug = false;
	}

	public void setDebug(boolean debug) {
		this.debug = debug;
	}

	public boolean isDebug() {
		return this.debug;
	}

	public void setSemanticInvoc(int semanticInvoc) {
		this.semanticInvoc = semanticInvoc;
	}

	public void setTimeoutMs(int timeoutMs) {
		this.timeoutMs = timeoutMs;
	}

	public int getTimeoutMs() {
		return this.timeoutMs;
	}

	public void setMaxRetry(int maxRetry) {
		this.maxRetry = maxRetry;
	}

	public int getNextRequestId() {
		this.requestIdCounter += 1;
		return this.requestIdCounter;
	}

	public ResponseDecision evaluateResponse(int responseId, boolean monitorMode) {
		boolean duplicate = this.semanticInvoc >= Constants.AT_MOST_ONE_SEM_INVO
				&& this.handledResponseIds.containsKey(responseId);
		boolean shouldAck = this.semanticInvoc >= Constants.AT_MOST_ONE_SEM_INVO
				|| (monitorMode && this.semanticInvoc >= Constants.AT_LEAST_ONE_SEM_INVO);

		if (!duplicate) {
			this.handledResponseIds.put(responseId, true);
		}

		return new ResponseDecision(duplicate, shouldAck);
	}

	public boolean shouldRetryAfterTimeout(int timeoutCount) throws TimeoutException, SocketTimeoutException {
		System.out.printf(Constants.TIMEOUT_MSG, timeoutCount, this.maxRetry);

		if (this.maxRetry > 0 && timeoutCount >= this.maxRetry) {
			throw new TimeoutException(Constants.MAX_TIMEOUT_MSG);
		}

		if (this.semanticInvoc < Constants.AT_LEAST_ONE_SEM_INVO) {
			throw new SocketTimeoutException("Semantic invocation mode is SEMANTIC_NONE");
		}

		return true;
	}
}
